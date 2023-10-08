#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "memory.h"
#include "logging.h"

#define MEMORY_BUS_ADDR (uint8_t)0x50

void mem_init()
{
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
}

int mem_read(uint8_t address, uint8_t *buffer, int len)
{
    uint8_t buff_addr[1];
    buff_addr[0] = address;
    int result = i2c_write_blocking(i2c_default, MEMORY_BUS_ADDR, buff_addr, 1, true);
    result = i2c_read_blocking(i2c_default, MEMORY_BUS_ADDR, buffer, len, false);
    sleep_us(500);
    return result;
}

int mem_save(uint8_t address, uint8_t *buffer, int len)
{
    int result = 0;
    size_t msglen = len + 1;
    uint8_t *record = malloc(msglen);
    if (record == NULL)
        return false;

    record[0] = address;
    memcpy(&record[1], buffer, len);

    result = i2c_write_blocking(i2c_default, MEMORY_BUS_ADDR, record, msglen, true);
    sleep_ms(5);
    result = mem_read(address, &record[1], len);
    if (memcmp(&record[1], &buffer[0], len))
    {
        log_message("Failure on saving memory");
    }

    free(record);
    sleep_ms(5);
    return result;
}

int mem_erase()
{
    int result = 0;
    for (int i = 0; i < 256; i++)
    {
        if ((result = mem_clear_byte(i)))
        {
            return 1;
        }
    }

    return result;
}

int mem_clear_byte(uint8_t address)
{
    char erase = 0xFF;
    return mem_save(address, &erase, 1);
}