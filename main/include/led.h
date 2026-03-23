#ifndef LED_H
#define LED_H

#include "driver/gpio.h"
#include "led_strip.h"
#include "sdkconfig.h"

#define BLINK_GPIO CONFIG_BLINK_GPIO

uint8_t get_led_state(void);
void led_on(void);
void led_off(void);
void led_init(void);

#endif
