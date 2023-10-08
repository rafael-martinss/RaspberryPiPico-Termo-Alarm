/**
 * @author Rafael Martins de Sousa - RA: 220023201
 * Este projeto consiste em operar como um alarme de temperatura.
 */

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "signalizer.h"

#define LED_STATUS_GPIO 21

void signalizer_init()
{
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    gpio_init(LED_STATUS_GPIO);
    gpio_set_dir(LED_STATUS_GPIO, GPIO_OUT);
    gpio_set_pulls(LED_STATUS_GPIO, true, false);
    signalizer_setAlarmOff();
}

void signalizer_setAlarmOn()
{
    gpio_put(LED_STATUS_GPIO, 0);
}

void signalizer_setAlarmOff()
{
    gpio_put(LED_STATUS_GPIO, 1);
}
