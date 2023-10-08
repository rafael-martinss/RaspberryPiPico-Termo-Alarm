/**
 * @author Rafael Martins de Sousa - RA: 220023201
 * Este projeto consiste em operar como um alarme de temperatura.
 *
 * O usuario seleciona o valor de trigger na inicialização.
 */

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "buttons.h"
#include "lcd.h"
#include "logging.h"
#include "termopar.h"
#include "signalizer.h"
#include "memory.h"

#define MEM_STATE_ADDR (uint8_t)0x20
#define MEM_TEMPERATURE_ADDR (uint8_t)0x32

typedef enum State
{
    STATE_CONFIG = 0xFF,
    STATE_MENU = 0xAA,
    STATE_RUN = 0x55,
} state_t;

static state_t mState = STATE_CONFIG;
uint8_t temperature_cfg = 25;
bool enter_pressed = false;

void show_temperature(float temperature)
{
    char message[16] = {0};
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_string("Temperature: ");
    lcd_set_cursor(1, 0);
    sprintf(message, "%.2f Celsius", temperature);
    lcd_string(message);
    printf("Current temperature: %.2f°C\n", temperature);
    signalizer_setAlarmOff();
}

void show_alarm(float temperature)
{
    char message[16] = {0};
    lcd_clear();
    lcd_set_cursor(0, 0);
    sprintf(message, "!ALARM! >= %d", temperature_cfg);
    lcd_string(message);
    lcd_set_cursor(1, 0);
    sprintf(message, "%.2f Celsius", temperature);
    lcd_string(message);
    printf("Current temperature: %.2f°C\n", temperature);
    signalizer_setAlarmOn();
}

void show_config_need()
{
    char message[16] = {0};
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_string("==SETUP  alarm==");
    lcd_set_cursor(1, 0);
    sprintf(message, "%d-(Enter=save)", temperature_cfg);
    lcd_string(message);
    if (enter_pressed)
    {
        enter_pressed = false;
        mState = STATE_RUN;
        mem_save(MEM_STATE_ADDR, (uint8_t *)&mState, 1);
        sleep_ms(10);
        mem_save(MEM_TEMPERATURE_ADDR, &temperature_cfg, 1);
        sleep_ms(10);
    }
}

void button_up_callback(void)
{
    log_message("Button Up pressed");
    temperature_cfg++;
}

void button_down_callback(void)
{
    log_message("Button Down pressed");
    temperature_cfg--;
}

void button_enter_callback(void)
{
    log_message("Button Enter pressed");
    enter_pressed = true;
}

int main()
{
    struct repeating_timer timer;
    float temperature = 0;

    stdio_init_all();
    cyw43_arch_init();
    sleep_ms(5000);

    log_message("Initializing memory...");
    mem_init();
    sleep_ms(100);

    // verify if the device is configured
    mem_read(MEM_STATE_ADDR, (uint8_t *)&mState, 1);
    mem_read(MEM_TEMPERATURE_ADDR, (uint8_t *)&temperature_cfg, 1);
    printf("mState: %X\n", mState);
    if (temperature_cfg == 0xFF)
    {
        temperature_cfg = 25;
    }
    printf("temperature_cfg: %X\n", temperature_cfg);
    sleep_ms(1000);

    // Initialize the LCD display
    log_message("Initializing LCD...");
    lcd_init();

    signalizer_init();

    // Initialize the temperature sensor
    log_message("Initializing Sensor...");
    termopar_init();

    // // Initialize the buttons
    buttons_init();
    buttons_setUpButtonCb(button_up_callback);
    buttons_setDownButtonCb(button_down_callback);
    buttons_setEnterButtonCb(button_enter_callback);

    while (true)
    {
        switch (mState)
        {
        default:
        case STATE_CONFIG:
        case STATE_MENU:
            show_config_need();
            break;

        case STATE_RUN:
            if (termopar_getStatus() != TERMOPAR_STATUS_OK)
            {
                sleep_ms(100);
                continue;
            }
            temperature = termopar_getTemperature();

            if (temperature >= temperature_cfg)
            {
                // raise the alarm!
                show_alarm(temperature);
            }
            else
            {
                show_temperature(temperature);
            }
            break;
        }
        sleep_ms(800);
    }
    return 0;
}
