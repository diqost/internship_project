#include <system_info.h>
#include "bluetoothleservice.h"
#include "bt/bt_common.h"

#define FEATURE_BLUETOOTH "http://tizen.org/feature/network.bluetooth"
#define FEATURE_BLUETOOTH_LE "http://tizen.org/feature/network.bluetooth.le"

/*
 * @brief: Check whether given feature is supported by the platform. This is a wrapper function
 * for the system_info_get_platform_bool() function.
 * @param[feature]: the feature's name to be checked.
 * @return: the function returns 'true' if the given feature is supported by the platform,
 * otherwise 'false' is returned.
 */
static bool __is_feature_supported(const char *feature)
{
	bool is_supported = false;
	int ret = system_info_get_platform_bool(feature, &is_supported);
	if (ret != SYSTEM_INFO_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function system_info_get_platform_bool() failed with error: %d.", get_error_message(ret));
		return false;
	}

	return is_supported;
}

/*
 * @brief: Check whether Bluetooth feature is supported by the platform.
 * The Bluetooth feature is specified as "http://tizen.org/feature/network.bluetooth".
 * @return: the function returns 'true' if the Bluetooth feature is supported by the platform,
 * otherwise 'false' is returned.
 */
bool bt_common_is_bluetooth_supported(void)
{
	return __is_feature_supported(FEATURE_BLUETOOTH);
}

/*
 * @brief: Check whether Bluetooth LE feature is supported by the platform.
 * The Bluetooth LE feature is specified as "http://tizen.org/feature/network.bluetooth.le".
 * @return: the function returns 'true' if the Bluetooth LE feature is supported by the platform,
 * otherwise 'false' is returned.
 */
bool bt_common_is_bluetooth_le_supported(void)
{
	return __is_feature_supported(FEATURE_BLUETOOTH_LE);
}

/*
 * @brief: The Bluetooth module is initialized. This function shall be called before any other
 * Bluetooth related functions.
 * @return: the function returns 'true' if the Bluetooth is initialized successfully,
 * otherwise 'false' is returned.
 */
bool bt_common_initialize(void)
{
	int ret = bt_initialize();
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_initialize() failed with error: %s.", get_error_message(ret));
		return false;
	}

	return true;
}

/*
 * @brief: The Bluetooth module is finalized. This function shall be called to close
 * the Bluetooth adapter. Afterwards any call to the Bluetooth will fail. This is a wrapper function
 * for the bt_deinitialize() function.
 * @return: the function returns 'true' if the Bluetooth is finalized successfully,
 * otherwise 'false' is returned.
 */
bool bt_common_finalize(void)
{
	int ret = bt_deinitialize();
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_deinitialize() failed with error: %s.", get_error_message(ret));
		return false;
	}

	return true;
}

/*
 * @brief: Function obtains the current state of the Bluetooth adapter. If the adapter was not
 * prior initialized, then it is assumed that the current state is disabled. This is a wrapper function
 * for the bt_adapter_get_state() function.
 * @param[is_enables]: the current state of the Bluetooth adapter.
 * @return: the function returns 'true' if the Bluetooth adapter's state was obtained successfully,
 * otherwise 'false' is returned.
 */
bool bt_common_get_state(bool *is_enabled)
{
	bt_adapter_state_e state = BT_ADAPTER_DISABLED;

	*is_enabled = false;

	int ret = bt_adapter_get_state(&state);
	if (ret == BT_ERROR_NOT_INITIALIZED)
		return true;

	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_adapter_get_state() failed with error: %s.", get_error_message(ret));
		return false;
	}

	*is_enabled = (state == BT_ADAPTER_ENABLED);

	return true;
}

/*
 * @brief: Function sets the callback function to monitor the state of the Bluetooth adapter.
 * Whenever the adapter's state is changed, the bound callback function is invoked. This is a wrapper function
 * for the bt_adapter_set_state_changed_cb() function.
 * @param[state_changed_cb]: the current state of the Bluetooth adapter.
 * @return: the function returns 'true' if the callback function was set successfully,
 * otherwise 'false' is returned.
 */
bool bt_common_set_state_change_callback(bt_adapter_state_changed_cb state_changed_cb)
{
	int ret = bt_adapter_set_state_changed_cb(state_changed_cb, NULL);
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_adapter_set_state_changed_cb() failed with error: %s.", get_error_message(ret));
		return false;
	}

	return true;
}

/*
 * @brief: Function removes the callback function previously set with the bt_common_set_state_change_callback().
 * This is a wrapper function for the bt_adapter_unset_state_changed_cb() function.
  * @return: the function returns 'true' if the callback function was unset successfully,
 * otherwise 'false' is returned.
 */
bool bt_common_unset_state_change_callback(void)
{
	int ret = bt_adapter_unset_state_changed_cb();
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_adapter_unset_state_changed_cb() failed with error: %s.", get_error_message(ret));
		return false;
	}

	return true;
}

/*
 * @brief: Function creates the Bluetooth LE advertiser. This is a wrapper function for the
 * bt_adapter_le_create_advertiser() function.
 * @param[adv_h]: the pointer to the handle of the newly created advertiser.
 * @return: the function returns 'true' if the Bluetooth LE advertiser was created successfully,
 * otherwise 'false' is returned.
 */
bool bt_common_create_advertizer(bt_advertiser_h *adv_h)
{
	int ret = bt_adapter_le_create_advertiser(adv_h);
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_adapter_le_create_advertiser() failed with error: %s.", get_error_message(ret));
		return false;
	}

	return true;
}

/*
 * @brief: Function deletes the Bluetooth LE advertiser prior created. This is a wrapper function for the
 * bt_adapter_le_destroy_advertiser() function.
 * @param[adv_h]: the handle of the prior created advertiser.
 * @return: the function returns 'true' if the Bluetooth LE advertiser was deleted successfully,
 * otherwise 'false' is returned.
 */
bool bt_common_delete_advertizer(bt_advertiser_h adv_h)
{
	int ret = bt_adapter_le_destroy_advertiser(adv_h);
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_adapter_le_destroy_advertiser() failed with error: %s.", get_error_message(ret));
		return false;
	}

	return true;
}

/*
 * @brief: Function sets the Bluetooth LE advertiser mode. This is a wrapper function for the
 * bt_adapter_le_set_advertising_mode() function.
 * @param[adv_h]: the handle of the prior created advertiser.
 * @param[mode]: the advertiser's mode to be set. Available modes are: BT_ADAPTER_LE_ADVERTISING_MODE_BALANCED,
 * BT_ADAPTER_LE_ADVERTISING_MODE_LOW_LATENCY, BT_ADAPTER_LE_ADVERTISING_MODE_LOW_ENERGY.
 * @return: the function returns 'true' if the Bluetooth LE advertiser's mode was set successfully,
 * otherwise 'false' is returned.
 */
bool bt_common_set_advertising_mode(bt_advertiser_h adv_h, bt_adapter_le_advertising_mode_e mode)
{
	int ret = bt_adapter_le_set_advertising_mode(adv_h, mode);
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_adapter_le_set_advertising_mode() failed with error: %s.", get_error_message(ret));
		return false;
	}

	return true;
}

/*
 * @brief: Function sets the Bluetooth LE advertiser connectable. This is a wrapper function for the
 * bt_adapter_le_set_advertising_connectable() function.
 * @param[adv_h]: the handle of the prior created advertiser.
 * @return: the function returns 'true' if the Bluetooth LE advertiser's became connectable,
 * otherwise 'false' is returned.
 */
bool bt_common_set_connectable(bt_advertiser_h adv_h)
{
	int ret = bt_adapter_le_set_advertising_connectable(adv_h, true);
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_adapter_le_set_advertising_connectable() failed with error: %s.", get_error_message(ret));
		return false;
	}

	return true;
}

/*
 * @brief: Function sets whether the Bluetooth adapter's name shall be included in transmitted packets. The device's name
 * is obtained internally by the Bluetooth API module and does not need to be set manually.
 * This is a wrapper function for the bt_adapter_le_set_advertising_device_name() function.
 * @param[adv_h]: the handle of the prior created advertiser.
 * @param[pkt_type]: the type of the packet to be affected. The available packet types are: BT_ADAPTER_LE_PACKET_ADVERTISING,
 * BT_ADAPTER_LE_PACKET_SCAN_RESPONSE.
 * @param[flag]: 'true' to include the Bluetooth device's name into the packet, 'false' otherwise.
 * @return: the function returns 'true' if the Bluetooth LE device's name inclusion was set successfully,
 * otherwise 'false' is returned.
 */
bool bt_common_set_device_name(bt_advertiser_h adv_h, bt_adapter_le_packet_type_e pkt_type, bool flag)
{
	int ret = bt_adapter_le_set_advertising_device_name(adv_h, pkt_type, flag);
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_adapter_le_set_advertising_device_name() failed with error: %s.", get_error_message(ret));
		return false;
	}

	return true;
}

/*
 * @brief: Function sets the Bluetooth LE appearance value. For possible identifiers, see the
 * https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.gap.appearance.xml
 * web page. This is a wrapper function for the bt_adapter_le_set_advertising_appearance() function.
 * @param[adv_h]: the handle of the prior created advertiser.
 * @param[pkt_type]: the type of the packet to be affected. The available packet types are: BT_ADAPTER_LE_PACKET_ADVERTISING,
 * BT_ADAPTER_LE_PACKET_SCAN_RESPONSE.
 * @param[appearance]: the appearance identifier.
 * @return: the function returns 'true' if the Bluetooth LE advertiser's appearance value was set successfully,
 * otherwise 'false' is returned.
 */
bool bt_common_set_appearance(bt_advertiser_h adv_h, bt_adapter_le_packet_type_e pkt_type, int appearance)
{
	int ret = bt_adapter_le_set_advertising_appearance(adv_h, pkt_type, appearance);
	if (ret  != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_adapter_le_set_advertising_appearance() failed with error: %s.", get_error_message(ret));
		return false;
	}

	return true;
}

/*
 * @brief: Function sets the Bluetooth device manufacturer identifier and a related data. For possible identifiers, see the
 * https://www.bluetooth.com/specifications/assigned-numbers/Company-Identifiers
 * web page. This is a wrapper function for the bt_adapter_le_add_advertising_manufacturer_data() function.
 * @param[adv_h]: the handle of the prior created advertiser.
 * @param[pkt_type]: the type of the packet to be affected. The available packet types are: BT_ADAPTER_LE_PACKET_ADVERTISING,
 * BT_ADAPTER_LE_PACKET_SCAN_RESPONSE.
 * @param[id]: the manufacturer identifier.
 * @param[data]: the general information on the manufacturer.
 * @param[data_size]: the size of the data buffer.
 * @return: the function returns 'true' if the Bluetooth device's manufacturer data was set successfully,
 * otherwise 'false' is returned.
 */
bool bt_common_set_manufacturer(bt_advertiser_h adv_h, bt_adapter_le_packet_type_e pkt_type, int id, const char *data, int data_size)
{
	int ret = bt_adapter_le_add_advertising_manufacturer_data(adv_h, pkt_type, id, data, data_size);
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_adapter_le_add_advertising_manufacturer_data() failed with error: %s.", get_error_message(ret));
		return false;
	}

	return true;
}

/*
 * @brief: Function sets whether the Bluetooth adapter's transmission power shall be included in transmitted packets. The value
 * of the transmission power is obtained internally by the Bluetooth API module and does not need to be set manually.
 * This is a wrapper function for the bt_adapter_le_set_advertising_tx_power_level() function.
 * @param[adv_h]: the handle of the prior created advertiser.
 * @param[pkt_type]: the type of the packet to be affected. The available packet types are: BT_ADAPTER_LE_PACKET_ADVERTISING,
 * BT_ADAPTER_LE_PACKET_SCAN_RESPONSE.
 * @param[flag]: 'true' to include the Bluetooth adapter's transmission power into the packet, 'false' otherwise.
 * @return: the function returns 'true' if the Bluetooth adapter's transmission power inclusion was set successfully,
 * otherwise 'false' is returned.
 */
bool bt_common_set_tx_power_level(bt_advertiser_h adv_h, bt_adapter_le_packet_type_e pkt_type, bool flag)
{
	int ret = bt_adapter_le_set_advertising_tx_power_level(adv_h, pkt_type, flag);
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_adapter_le_set_advertising_tx_power_level() failed with error: %s.", get_error_message(ret));
		return false;
	}

	return true;
}

/*
 * @brief: Function adds the solicitation UUID to the advertised packets. This UUID identifies the data carried by the packet.
 * This is a wrapper function for the bt_adapter_le_add_advertising_service_solicitation_uuid() function.
 * @param[adv_h]: the handle of the prior created advertiser.
 * @param[pkt_type]: the type of the packet to be affected. The available packet types are: BT_ADAPTER_LE_PACKET_ADVERTISING,
 * BT_ADAPTER_LE_PACKET_SCAN_RESPONSE.
 * @param[uuid]: the UUID identifier of the data.
 * @return: the function returns 'true' if the UUID was added successfully, otherwise 'false' is returned.
 */
bool bt_common_add_service_solicitation_uuid(bt_advertiser_h adv_h, bt_adapter_le_packet_type_e pkt_type, const char *uuid)
{
	int ret = bt_adapter_le_add_advertising_service_solicitation_uuid(adv_h, pkt_type, uuid);
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_adapter_le_add_advertising_service_solicitation_uuid() failed with error: %s.", get_error_message(ret));
		return false;
	}

	return true;
}

/*
 * @brief: Function sets the data correlated to the UUID prior added with the bt_common_add_service_solicitation_uuid() function.
 * The provided data is bound to the UUID and advertised afterwards the Bluetooth LE advertiser is started.
 * This is a wrapper function for the bt_adapter_le_add_advertising_service_data() function.
 * @param[adv_h]: the handle of the prior created advertiser.
 * @param[pkt_type]: the type of the packet to be affected. The available packet types are: BT_ADAPTER_LE_PACKET_ADVERTISING,
 * BT_ADAPTER_LE_PACKET_SCAN_RESPONSE.
 * @param[uuid]: the UUID identifier of the data.
 * @param[service_data]: the data buffer to be added to the advertised packets and correlated with the UUID.
 * @param[service_data_len]: the length of the 'service_data' buffer.
 * @return: the function returns 'true' if the data was set successfully, otherwise 'false' is returned.
 */
bool bt_common_set_service_data(bt_advertiser_h advertiser, bt_adapter_le_packet_type_e pkt_type, const char *uuid, const char *service_data, int service_data_len)
{
	int ret = bt_adapter_le_add_advertising_service_data(advertiser, pkt_type, uuid, service_data, service_data_len);
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_adapter_le_add_advertising_service_data() failed with error: %s.", get_error_message(ret));
		return false;
	}

	return true;
}

/*
 * @brief: Function starts the service's data broadcasting by the Bluetooth LE advertiser.
 * This is a wrapper function for the bt_adapter_le_start_advertising_new() function.
 * @param[adv_h]: the handle of the prior created advertiser.
 * @param[state_changed_cb]: the callback function handler invoked on advertiser's state change.
 * @return: the function returns 'true' if the advertiser was started successfully, otherwise 'false' is returned.
 */
bool bt_common_start_advertising(bt_advertiser_h adv_h, bt_adapter_le_advertising_state_changed_cb state_changed_cb)
{
	int ret = bt_adapter_le_start_advertising_new(adv_h, state_changed_cb, NULL);
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_adapter_le_start_advertising_new() failed with error: %s.", get_error_message(ret));
		return false;
	}

	return true;
}

/*
 * @brief: Function stops the service's data broadcasting by the Bluetooth LE advertiser.
 * This is a wrapper function for the bt_adapter_le_stop_advertising() function.
 * @param[adv_h]: the handle of the prior created advertiser.
 * @return: the function returns 'true' if the advertiser was stopped successfully, otherwise 'false' is returned.
 */
bool bt_common_stop_advertising(bt_advertiser_h adv_h)
{
	int ret = bt_adapter_le_stop_advertising(adv_h);
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_adapter_le_stop_advertising() failed with error: %s.", get_error_message(ret));
		return false;
	}

	return true;
}

/*
 * @brief: Function clears all the advertiser's settings and data.
 * This is a wrapper function for the bt_adapter_le_clear_advertising_data() function.
 * @param[adv_h]: the handle of the prior created advertiser.
 * @param[pkt_type]: the type of the packet to be affected. The available packet types are: BT_ADAPTER_LE_PACKET_ADVERTISING,
 * BT_ADAPTER_LE_PACKET_SCAN_RESPONSE.
 * @return: the function returns 'true' if the advertiser was cleared successfully, otherwise 'false' is returned.
 */
bool bt_common_clear_advertizer(bt_advertiser_h adv_h, bt_adapter_le_packet_type_e pkt_type)
{
	int ret = bt_adapter_le_clear_advertising_data(adv_h, pkt_type);
	if (ret != BT_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Function bt_adapter_le_clear_advertising_data() failed with error: %s.", get_error_message(ret));
		return false;
	}

	return true;
}
