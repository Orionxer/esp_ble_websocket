file(GLOB_RECURSE srcs "ble_websocket.c" "src/*.c")

idf_component_register(
    SRCS "${srcs}"
    EMBED_TXTFILES "AAA-Certificate-Services.pem"
    EMBED_FILES "starship_1mb.mp4"
    INCLUDE_DIRS
        "."
        "./include"
        "${IDF_PATH}/components/bt/host/nimble/nimble/nimble/include"
        "${IDF_PATH}/components/bt/host/nimble/nimble/nimble/host/include"
        "${IDF_PATH}/components/bt/host/nimble/nimble/nimble/host/services/gap/include"
        "${IDF_PATH}/components/bt/host/nimble/nimble/nimble/host/services/gatt/include"
    REQUIRES esp_event esp_netif esp_wifi esp_websocket_client mbedtls nvs_flash bt esp_driver_gpio
)
