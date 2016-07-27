/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !defined(_BT_COMMON_H_)
#define _BT_COMMON_H_

#include <bluetooth.h>
#include <stdbool.h>

bool bt_common_is_bluetooth_supported(void);
bool bt_common_is_bluetooth_le_supported(void);
bool bt_common_initialize(void);
bool bt_common_finalize(void);
bool bt_common_get_state(bool *is_enabled);
bool bt_common_set_state_change_callback(bt_adapter_state_changed_cb state_changed_cb);
bool bt_common_unset_state_change_callback(void);
bool bt_common_create_advertizer(bt_advertiser_h *adv_h);
bool bt_common_delete_advertizer(bt_advertiser_h adv_h);
bool bt_common_set_advertising_mode(bt_advertiser_h adv_h, bt_adapter_le_advertising_mode_e mode);
bool bt_common_set_connectable(bt_advertiser_h adv_h);
bool bt_common_set_device_name(bt_advertiser_h adv_h, bt_adapter_le_packet_type_e pkt_type, bool flag);
bool bt_common_set_appearance(bt_advertiser_h adv_h, bt_adapter_le_packet_type_e pkt_type, int appearance);
bool bt_common_set_manufacturer(bt_advertiser_h adv_h, bt_adapter_le_packet_type_e pkt_type, int id, const char *data, int data_size);
bool bt_common_set_tx_power_level(bt_advertiser_h adv_h, bt_adapter_le_packet_type_e pkt_type, bool flag);
bool bt_common_add_service_solicitation_uuid(bt_advertiser_h adv_h, bt_adapter_le_packet_type_e pkt_type, const char *uuid);
bool bt_common_set_service_data(bt_advertiser_h adv_h, bt_adapter_le_packet_type_e pkt_type, const char *uuid, const char *service_data, int service_data_len);
bool bt_common_start_advertising(bt_advertiser_h adv_h, bt_adapter_le_advertising_state_changed_cb state_changed_cb);
bool bt_common_stop_advertising(bt_advertiser_h adv_h);
bool bt_common_clear_advertizer(bt_advertiser_h adv_h, bt_adapter_le_packet_type_e pkt_type);

#endif
