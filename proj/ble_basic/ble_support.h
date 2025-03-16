#ifndef BLE_SUPPORT_H
#define BLE_SUPPORT_H

#include <stdint.h>
#include <stdbool.h>

#include "pico/cyw43_arch.h"

///////////////////////////////////////////////////////////////////////////

#define USR_LED_ON()   cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1)
#define USR_LED_OFF()  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0)

///////////////////////////////////////////////////////////////////////////

bool ble_support_init();

#endif // BLE_SUPPORT
