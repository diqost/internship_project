#include "bluetoothleservice.h"
#include "bt/bt_advertiser.h"
#include "bt/bt_common.h"

#define APPEARANCE_GENERIC_HEART_RATE_SENSOR 832
#define MANUFACTURER_ID_SAMSUNG 0x0075

struct _manufacturer_data {
	unsigned short id;
};

struct _service_data {
	unsigned short id;
};

struct _adv_properties {
	bt_adapter_le_packet_type_e pkt_type;
	bool name;
	bool tx_power;
	int appearance;
	int manufacturer_id;
	struct _manufacturer_data manufacturer_data;
	int servicedata_id;
	struct _service_data service_data;
};

static int __advertizer_appearance = 0;

/*
 * @brief: Internal function which sets the base properties of the advertiser prior created.
 * The following properties are set: Bluetooth adapter's name inclusion flag, adapter's transmission
 * power level inclusion flag, adapter's appearance value and manufacturers id and related data.
 * All of these properties are set for the packet type specified by 'pkt_type' member of the
 * 'properties' structure. The available packet types are: BT_ADAPTER_LE_PACKET_ADVERTISING,
 * BT_ADAPTER_LE_PACKET_SCAN_RESPONSE.
 * @param[adv_h]: the handle of the advertiser to be altered.
 * @param[properties]: the structure of properties to be set to the advertiser.
 * @return: the function returns 'true' if the properties were set successfully,
 * otherwise 'false' is returned.
 */
static bool __set_properties(bt_advertiser_h adv_h, struct _adv_properties properties)
{
	/*if(properties.name)
		if (!bt_common_set_device_name(adv_h, properties.pkt_type, properties.name))
			return false;*/

	if (properties.appearance > 0)
		if (!bt_common_set_appearance(adv_h, properties.pkt_type, properties.appearance))
			return false;

	if (properties.manufacturer_id > 0)
		if (!bt_common_set_manufacturer(adv_h,
										properties.pkt_type,
										properties.manufacturer_id,
										(const char *)&properties.manufacturer_data,
										sizeof(struct _manufacturer_data)))
			return false;
/*
	if (!bt_common_set_tx_power_level(adv_h, properties.pkt_type, properties.tx_power))
		return false;*/

	return true;
}

/*
 * @brief: Internal function which creates the base properties of the prior created advertiser for
 * BT_ADAPTER_LE_PACKET_ADVERTISING packet type. This function invokes the __set_properties() with
 * perdefined properties structure.
 * @param[adv_h]: the handle of the advertiser to be altered.
 * @return: the function returns 'true' if the properties were set successfully,
 * otherwise 'false' is returned.
 */
bool __set_advertizing_properties(bt_advertiser_h adv_h)
{
	struct _adv_properties properties = {0,};

	properties.pkt_type = BT_ADAPTER_LE_PACKET_ADVERTISING;
	properties.name = true;
	properties.tx_power = false;
	properties.appearance = 0;
	properties.manufacturer_id = MANUFACTURER_ID_SAMSUNG;
	properties.manufacturer_data.id = MANUFACTURER_ID_SAMSUNG;

	return __set_properties(adv_h, properties);
}

/*
 * @brief: Internal function which creates the base properties of the prior created advertiser for
 * BT_ADAPTER_LE_PACKET_SCAN_RESPONSE packet type. This function invokes the __set_properties() with
 * perdefined properties structure.
 * @param[adv_h]: the handle of the advertiser to be altered.
 * @param[appearance]: the value of the device's appearance.
 * @return: the function returns 'true' if the properties were set successfully,
 * otherwise 'false' is returned.
 */
bool __set_scan_response_properties(bt_advertiser_h adv_h, int appearance)
{
	struct _adv_properties properties = {0,};

	properties.pkt_type = BT_ADAPTER_LE_PACKET_SCAN_RESPONSE;
	properties.name = true;
	properties.tx_power = false;
	properties.appearance = appearance;
	properties.manufacturer_id = MANUFACTURER_ID_SAMSUNG;
	properties.manufacturer_data.id = MANUFACTURER_ID_SAMSUNG;

	return __set_properties(adv_h, properties);
}

/*
 * @brief: Internal function which creates the Bluetooth LE advertiser and sets its properties for all
 * available packet types.
 * @param[adv_h]: the handle of the advertiser created.
 * @param[appearance]: the value of the device's appearance added to the BT_ADAPTER_LE_PACKET_SCAN_RESPONSE
 * packet type only.
 * @return: the function returns 'true' if the advertiser was created and configured successfully,
 * otherwise 'false' is returned.
 */
static bool __create_advertizer(bt_advertiser_h *adv_h, int appearance)
{
	*adv_h = NULL;

	if (!bt_common_create_advertizer(adv_h))
		return false;

	if (!bt_common_set_advertising_mode(*adv_h, BT_ADAPTER_LE_ADVERTISING_MODE_LOW_LATENCY))
		return false;

	if (!bt_common_set_connectable(*adv_h))
		return false;

	if (!__set_advertizing_properties(*adv_h))
		return false;

	if (!__set_scan_response_properties(*adv_h, appearance))
		return false;

	return true;
}

/*
 * @brief: Internal function which deletes the Bluetooth LE advertiser.
 * @param[adv_h]: the handle of the advertiser to be deleted.
 */
static void __delete_advertizer(bt_advertiser_h *adv_h)
{
	bt_common_delete_advertizer(*adv_h);
	*adv_h = NULL;
	__advertizer_appearance = 0;
}

/*
 * @brief: Function creates the Bluetooth LE advertiser with preset properties and starts the advertising.
 * @param[adv_h]: the handle of the advertiser created.
 * @param[appearance]: the value of the device's appearance added to the BT_ADAPTER_LE_PACKET_SCAN_RESPONSE
 * packet type only.
 * @return: the function returns 'true' if the advertiser was created and started successfully,
 * otherwise 'false' is returned.
 */
bool bt_advertiser_create(bt_advertiser_h *adv_h, int appearance)
{
	if (!__create_advertizer(adv_h, appearance))
		return false;

	if (!bt_common_start_advertising(*adv_h, NULL)) {
		__delete_advertizer(adv_h);
		return false;
	}

	__advertizer_appearance = appearance;

	return true;
}

/*
 * @brief: Function stops and deletes the Bluetooth LE advertiser.
 * @param[adv_h]: the handle of the advertiser to be deleted.
 */
void bt_advertiser_delete(bt_advertiser_h *adv_h)
{
	bt_common_stop_advertising(*adv_h);
	__delete_advertizer(adv_h);
}

/*
 * @brief: Function sets the custom data identified with provided UUID to the advertiser's BT_ADAPTER_LE_PACKET_SCAN_RESPONSE packet.
 * Before the advertiser is updated, it has to be stopped and cleared. Once the advertiser is cleared, it has to be configured again
 * by provision of relevant properties. Afterwards the solicitation UUID is added and the provided data bound to this UUID.
 * Finally the advertiser is started again.
 * @param[adv_h]: the handle of the advertiser to be altered.
 * @param[uuid]: the UUID of the service's to be correlated with the provided data.
 * @param[data]: the custom data array to be added.
 * @param[data_size]: the size of the data passed.
 * @return: the function returns 'true' if the data was successfully added to the advertiser,
 * otherwise 'false' is returned.
 */
bool bt_advertizer_set_data(bt_advertiser_h *adv_h, const char *uuid, const char *data, int data_size)
{
	bt_common_stop_advertising(*adv_h);

	if (bt_common_clear_advertizer(*adv_h, BT_ADAPTER_LE_PACKET_SCAN_RESPONSE)) {
		__set_scan_response_properties(*adv_h, __advertizer_appearance);

		if (bt_common_add_service_solicitation_uuid(*adv_h, BT_ADAPTER_LE_PACKET_SCAN_RESPONSE, uuid))
			if (data_size > 0)
				bt_common_set_service_data(*adv_h, BT_ADAPTER_LE_PACKET_SCAN_RESPONSE, uuid, data, data_size);
	}

	bt_common_start_advertising(*adv_h, NULL);

	return true;
}
