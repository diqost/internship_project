#include <tizen.h>
#include <service_app.h>
#include <message_port.h>
#include <dlog.h>
#include <locale.h>
#include <stdlib.h>
#include "bluetoothleservice.h"
#include "bt/bt_common.h"
#include "bt/bt_advertiser.h"
#include "servicedata.h"


struct _ctrl_data {
	bt_advertiser_h adv_h;	/* advertiser handle */
};

static struct _ctrl_data __ctrldata = {0,};


const char* local_port = "BLE_NATIVE";
const char* global_remote_port = "BLE_WEB";
const char* remote_app_id = "dsdml9mCw4.LinkedMaps";
int local_port_id;

void _bluetooth_adv(void* data);
static bool __bt_init(void);
static void __bt_device_state_changed_cb(int result, bt_adapter_state_e adapter_state, void *user_data);
static void __bt_adapter_le_scan_result_cb(int result, bt_adapter_le_device_scan_result_info_s *info, void *user_data);


// Daemon gets message from WEB
void message_port_cb(int local_port_id, const char *remote_app_id, const char *remote_port,
                bool trusted_remote_port, bundle *message, void *user_data)
{
    bundle *reply = bundle_create();
    int ret;
    char *command = NULL;
    char *data = NULL;
    char *str = NULL;

 //   _sdata *sdata = (_sdata *) memset(sizeof(_sdata));
    bundle_get_str(message, "command", &command);


    dlog_print(DLOG_INFO, TAG, "Message from %s (%s), command: %s",
               remote_app_id, remote_port, command);

if ( strcmp(command, "startScan") == 0 )
{
	dlog_print(DLOG_INFO, LOG_TAG, "Starting scan");
    ret = BT_ERROR_NONE;
   	    ret = bt_adapter_le_start_scan(__bt_adapter_le_scan_result_cb, NULL);

   	    bundle *reply = bundle_create();

   	    if (ret != BT_ERROR_NONE)    {
   	    	bundle_add_str(reply, "result", "Failed");
   	        dlog_print(DLOG_ERROR, LOG_TAG, "[bt_adapter_le_start_scan] failed.");
   	    }
   	    else
   	    	bundle_add_str(reply, "result", "OK");
   	    return;
}

else if ( strcmp(command, "stopScan") == 0 )
{
	dlog_print(DLOG_INFO, LOG_TAG, "Stopping scan");
    ret = BT_ERROR_NONE;
   	    ret = bt_adapter_le_stop_scan();

   	    if (ret != BT_ERROR_NONE)    {
   	    	bundle_add_str(reply, "result", "Failed");
   	        dlog_print(DLOG_ERROR, LOG_TAG, "[bt_adapter_le_start_stop] failed.");
   	    }
   	    else
   	    	bundle_add_str(reply, "result", "OK");

   	    return;
}

else if ( strcmp(command, "setAdv") == 0 )
{

	_sdata *sdata = malloc(sizeof(_sdata));
	 char *str;
	// bundle_get_str(message, "lat", &str);
	// sdata->lat = strtof(str, NULL);


	sdata->command = 8;
	sdata->lat = 243.12f;
	sdata->lon = 121.3456f;

	if (!bt_advertizer_set_data(__ctrldata.adv_h, SERVICE_UUID , (const char *) sdata, sizeof(_sdata)))
	{
		bundle_add_str(reply, "result", "Failed");
		dlog_print(DLOG_ERROR, TAG, "Service data error.");
	}
	else
		bundle_add_str(reply, "result", "OK");

	free(sdata);
	 return;
}

  //  bundle_get_str(message, "data", &data);


    ret = message_port_send_message(remote_app_id, global_remote_port, reply);


    if (ret != MESSAGE_PORT_ERROR_NONE)
        dlog_print(DLOG_ERROR, TAG, "Port send message error: %s", get_error_message(ret));

    bundle_free(reply);
    //free(sdata);
}

bool service_app_create(void *data)
{
	int ret;
	bool found;

	dlog_print(DLOG_INFO, TAG, "Service started");
	setlocale(LC_NUMERIC, "C");

	local_port_id = message_port_register_local_port(local_port, message_port_cb, NULL);
	if (local_port_id < 0)
	    dlog_print(DLOG_ERROR, TAG, "Port register error: %d", local_port_id);
	else
	    dlog_print(DLOG_INFO, TAG, "port_id: %d", local_port_id);


	ret = message_port_check_remote_port(remote_app_id, global_remote_port, &found);
	if (ret != MESSAGE_PORT_ERROR_NONE)
	    dlog_print(DLOG_ERROR, TAG, "message_port_check_remote_port error: %d", ret);


	if (__bt_init())
		dlog_print(DLOG_INFO, TAG, "Bluetooth activated.");
	else
		dlog_print(DLOG_ERROR, TAG, "Bluetooth activation error.");

    return true;
}

void service_app_terminate(void *data)
{

	message_port_unregister_local_port (local_port_id);

	bt_advertiser_delete(__ctrldata.adv_h);
	dlog_print(DLOG_INFO, TAG, "Service terminated");

/*	 int ret = bt_adapter_le_stop_scan();
	    if (ret != BT_ERROR_NONE) {
	        dlog_print(DLOG_ERROR, LOG_TAG, "[bt_adapter_le_stop_scan] failed.");
	    }*/

    return;
}

void service_app_control(app_control_h app_control, void *data)
{
    // Todo: add your code here.
    return;
}

static void
service_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	return;
}

static void
service_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
service_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
service_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int main(int argc, char* argv[])
{
    char ad[50] = {0,};
	service_app_lifecycle_callback_s event_callback;
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	service_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, service_app_low_battery, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, service_app_low_memory, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, service_app_lang_changed, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, service_app_region_changed, &ad);

	return service_app_main(argc, argv, &event_callback, ad);
}



/*
 * @brief: Initialization of Bluetooth adapter and its advertiser.
 * @return: Function returns 'true' if the Bluetooth adapter and its advertiser
 * is successfully initialized, otherwise 'false' is returned.
 */
static bool __bt_init(void)
{
	bool is_enabled = false;

	if (bt_common_initialize()) {
		bt_common_set_state_change_callback(__bt_device_state_changed_cb);

		bt_common_get_state(&is_enabled);

		if (bt_advertiser_create(&__ctrldata.adv_h, 132))
			dlog_print(DLOG_INFO, TAG, "advertiser started.");
		else
			dlog_print(DLOG_ERROR, TAG, "advertiser start failed.");
	}

	return is_enabled;
}

static void __bt_device_state_changed_cb(int result, bt_adapter_state_e adapter_state, void *user_data)
{

	bool enabled = false;
	bt_common_get_state(&enabled);

	if (enabled) {
		if (bt_advertiser_create(&__ctrldata.adv_h, 132))
			dlog_print(DLOG_INFO, TAG, "advertiser started.");
		else
			dlog_print(DLOG_WARN, TAG, "Failed to start advertiser.");
	} else if (__ctrldata.adv_h) {
		bt_advertiser_delete(&__ctrldata.adv_h);
		dlog_print(DLOG_WARN, TAG, "advertiser stopped.");
	}
	//data_change_advertiser_state(adapter_state == BT_ADAPTER_ENABLED);
}

// Daemon gets adv-packet
static void
__bt_adapter_le_scan_result_cb(int result,
                               bt_adapter_le_device_scan_result_info_s *info,
                               void *user_data)
{
	//int i;
	bundle *message = NULL;
    bt_adapter_le_packet_type_e pkt_type =  BT_ADAPTER_LE_PACKET_SCAN_RESPONSE;//BT_ADAPTER_LE_PACKET_ADVERTISING;

    if (info == NULL) {
        dlog_print(DLOG_INFO, LOG_TAG, "No discovery_info!");

        return;
    }

    if (info->adv_data_len > 31 || info->scan_data_len > 31) {
        dlog_print (DLOG_INFO, LOG_TAG, "###################");
        bt_adapter_le_stop_scan();
        dlog_print (DLOG_INFO, LOG_TAG, "###################");

        return;
    }

  //  for (i = 0; i < 2; i++)
  //  {
        char **uuids;
        char *device_name;
        int tx_power_level;
        bt_adapter_le_service_data_s *data_list;
        int appearance;
        int manufacturer_id;
        char *manufacturer_data;
        int manufacturer_data_len;
        int count;
      	 _sdata *sdata = NULL;
      //  pkt_type += i;
       // if (pkt_type == BT_ADAPTER_LE_PACKET_ADVERTISING && info->adv_data == NULL)
        //    continue;
      //  if (pkt_type == BT_ADAPTER_LE_PACKET_SCAN_RESPONSE && info->scan_data == NULL)
      //      break;

     /*   if (bt_adapter_le_get_scan_result_service_uuids(info, pkt_type, &uuids, &count) == BT_ERROR_NONE) {
            int i;
            for (i = 0; i < count; i++)
            {
                dlog_print(DLOG_INFO, LOG_TAG, "UUID[%d] = %s", i + 1, uuids[i]);
               //g_
                free(uuids[i]);
            }
           //g_
            free(uuids);
        }
        */

        if (bt_adapter_le_get_scan_result_device_name(info, pkt_type, &device_name) == BT_ERROR_NONE) {
            dlog_print(DLOG_INFO, LOG_TAG, "Device name = %s", device_name);
            //g_
            //free(device_name);
        }
        if (bt_adapter_le_get_scan_result_tx_power_level(info, pkt_type, &tx_power_level) == BT_ERROR_NONE) {
            dlog_print(DLOG_INFO, LOG_TAG, "TX Power level = %d", tx_power_level);
        }
        if (bt_adapter_le_get_scan_result_service_solicitation_uuids(info, pkt_type, &uuids, &count) == BT_ERROR_NONE) {
            int i;
            for (i = 0; i < count; i++)
            {
                dlog_print(DLOG_INFO, LOG_TAG, "Solicitation UUID[%d] = %s", i + 1, uuids[i]);
                //g_
                free(uuids[i]);
            }
           // g_
            free(uuids);
        }
        if (bt_adapter_le_get_scan_result_service_data_list(info, pkt_type, &data_list, &count) == BT_ERROR_NONE) {
            int i;
            for (i = 0; i < count; i++)
            {
            	dlog_print(DLOG_INFO, LOG_TAG, "Service Data[%x] = [%i] (%f, f)",// i + 1,
                    data_list[i].service_uuid, sdata->command, sdata->lat, sdata->lon);

            	if(data_list[i].service_uuid == SERVICE_UUID)
            	{
            		sdata = (_sdata*) malloc(data_list[i].service_data_len);
            		memcpy(sdata, data_list[i].service_data, data_list[i].service_data_len);





            		free(sdata);
            	}

            bt_adapter_le_free_service_data_list(data_list, count);
            }
        }



        if (bt_adapter_le_get_scan_result_appearance(info, pkt_type, &appearance) == BT_ERROR_NONE) {
            dlog_print(DLOG_INFO, LOG_TAG, "Appearance = %d", appearance);
        }
        if (bt_adapter_le_get_scan_result_manufacturer_data(info, pkt_type, &manufacturer_id, &manufacturer_data, &manufacturer_data_len) == BT_ERROR_NONE) {
            dlog_print(DLOG_INFO, LOG_TAG, "Manufacturer data[ID:%.4X, 0x%.2X%.2X...(len:%d)]",
                    manufacturer_id, manufacturer_data[0], manufacturer_data[1], manufacturer_data_len);
           // g_
            free(manufacturer_data);
        }

   // }

        // bundle_free(bundle);
}


