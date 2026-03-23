#include "ble_app.h"

#include "common.h"
#include "gap.h"
#include "gatt_svc.h"
#include "heart_rate.h"
#include "led.h"

void ble_store_config_init(void);

static bool s_ble_started;

static void on_stack_reset(int reason)
{
    ESP_LOGI(TAG, "nimble stack reset, reset reason: %d", reason);
}

static void on_stack_sync(void)
{
    adv_init();
}

static void nimble_host_config_init(void)
{
    ble_hs_cfg.reset_cb = on_stack_reset;
    ble_hs_cfg.sync_cb = on_stack_sync;
    ble_hs_cfg.gatts_register_cb = gatt_svr_register_cb;
    ble_hs_cfg.store_status_cb = ble_store_util_status_rr;

    ble_store_config_init();
}

static void nimble_host_task(void *param)
{
    ESP_LOGI(TAG, "nimble host task has been started!");
    nimble_port_run();
    vTaskDelete(NULL);
}

static void heart_rate_task(void *param)
{
    ESP_LOGI(TAG, "heart rate task has been started!");

    while (1) {
        update_heart_rate();
        ESP_LOGI(TAG, "heart rate updated to %d", get_heart_rate());
        send_heart_rate_indication();
        vTaskDelay(HEART_RATE_TASK_PERIOD);
    }
}

esp_err_t ble_websocket_nvs_init(void)
{
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "failed to initialize nvs flash, error code: %d", ret);
    }

    return ret;
}

esp_err_t ble_app_start(void)
{
    int rc;
    esp_err_t ret;

    if (s_ble_started) {
        ESP_LOGW(TAG, "BLE app already started");
        return ESP_OK;
    }

    led_init();

    ret = nimble_port_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "failed to initialize nimble stack, error code: %d", ret);
        return ret;
    }

#if CONFIG_BT_NIMBLE_GAP_SERVICE
    rc = gap_init();
    if (rc != 0) {
        ESP_LOGE(TAG, "failed to initialize GAP service, error code: %d", rc);
        return ESP_FAIL;
    }
#endif

    rc = gatt_svc_init();
    if (rc != 0) {
        ESP_LOGE(TAG, "failed to initialize GATT server, error code: %d", rc);
        return ESP_FAIL;
    }

    nimble_host_config_init();

    xTaskCreate(nimble_host_task, "NimBLE Host", 4 * 1024, NULL, 5, NULL);
    xTaskCreate(heart_rate_task, "Heart Rate", 4 * 1024, NULL, 5, NULL);

    s_ble_started = true;
    return ESP_OK;
}
