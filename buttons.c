/**
 * @author Rafael Martins de Sousa - RA: 220023201
 * Este projeto consiste em operar como um alarme de temperatura.
 */

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#define BUTTON_ENTER_GPIO 10
#define BUTTON_UP_GPIO 14
#define BUTTON_DOWN_GPIO 15

typedef void (*button_up_callback_t)(void);
typedef void (*button_down_callback_t)(void);
typedef void (*button_enter_callback_t)(void);

button_up_callback_t up_callback = NULL;
button_down_callback_t down_callback = NULL;
button_enter_callback_t enter_callback = NULL;

void gpio_callback(uint gpio, uint32_t events)
{
    (void)events;
    switch (gpio)
    {
    case BUTTON_UP_GPIO:
        if (up_callback)
        {
            (*up_callback)();
        }
        break;
    case BUTTON_DOWN_GPIO:
        if (down_callback)
        {
            (*down_callback)();
        }
        break;
    case BUTTON_ENTER_GPIO:
        if (enter_callback)
        {
            (*enter_callback)();
        }
        break;

    default:
        break;
    }
}

void buttons_init()
{
    gpio_init(BUTTON_ENTER_GPIO);
    gpio_set_dir(BUTTON_ENTER_GPIO, GPIO_IN);
    gpio_set_pulls(BUTTON_ENTER_GPIO, true, false);
    gpio_set_irq_enabled_with_callback(BUTTON_ENTER_GPIO, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    gpio_init(BUTTON_UP_GPIO);
    gpio_set_dir(BUTTON_UP_GPIO, GPIO_IN);
    gpio_set_pulls(BUTTON_UP_GPIO, true, false);
    gpio_set_irq_enabled_with_callback(BUTTON_UP_GPIO, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    gpio_init(BUTTON_DOWN_GPIO);
    gpio_set_dir(BUTTON_DOWN_GPIO, GPIO_IN);
    gpio_set_pulls(BUTTON_DOWN_GPIO, true, false);
    gpio_set_irq_enabled_with_callback(BUTTON_DOWN_GPIO, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
}

void buttons_setUpButtonCb(button_up_callback_t callback)
{
    up_callback = callback;
}

void buttons_setDownButtonCb(button_down_callback_t callback)
{
    down_callback = callback;
}

void buttons_setEnterButtonCb(button_enter_callback_t callback)
{
    enter_callback = callback;
}
