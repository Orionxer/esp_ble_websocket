#ifndef BLE_APP_H
#define BLE_APP_H

#include "esp_err.h"

esp_err_t ble_websocket_nvs_init(void);
esp_err_t ble_app_start(void);

#endif
