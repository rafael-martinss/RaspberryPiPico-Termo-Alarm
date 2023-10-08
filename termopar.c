/**
 * @author Rafael Martins de Sousa - RA: 220023201
 * Este projeto consiste em operar como um alarme de temperatura.
 */

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/spi.h"

#include "logging.h"
#include "termopar.h"

#define TEMPERATURE_DATA_SIZE 2
#define TEMPERATURE_DATA_MSB_OFFSET 0
#define TEMPERATURE_DATA_LSB_OFFSET 1
#define TEMPERATURE_DATA_ERROR_FLAG_OFFSET 1
#define TEMPERATURE_DATA_ERROR_DATA_OFFSET 3
#define TEMPERATURE_DATA_DEC_PORTION_SHIFT 2
#define TEMPERATURE_DATA_INT_PORTION_SHIFT 4
#define TEMPERATURE_MASK 0b00001100
#define TEMPERATURE_SENSOR_ERROR_MASK 0b00000001
#define TEMPERATURE_SENSOR_ERROR_OPEN_MASK 0b00000001
#define TEMPERATURE_SENSOR_ERROR_SC_GND_MASK 0b00000010
#define TEMPERATURE_SENSOR_ERROR_SC_VCC_MASK 0b00000100
#define TEMPERATURE_FACTOR 0.25

#ifdef MAX6675
#warning "Termopar using MAX6675"
#else
#warning "Termopar using MAX31856"
#endif

void termopar_init(void)
{
    spi_init(spi_default, 5000 * 1000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);

    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, true);
}

float termopar_getTemperature(void)
{
    uint8_t data[TEMPERATURE_DATA_SIZE];

    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);
    sleep_us(10);
    spi_read_blocking(spi_default, 0, data, TEMPERATURE_DATA_SIZE);
    sleep_us(10);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);

#ifdef MAX6675
    int integer = (((data[TEMPERATURE_DATA_MSB_OFFSET] << 8) | data[TEMPERATURE_DATA_LSB_OFFSET]) >> 3);
    return TEMPERATURE_FACTOR * integer;
#else
    float decimal = ((data[TEMPERATURE_DATA_LSB_OFFSET] & TEMPERATURE_MASK) >> TEMPERATURE_DATA_DEC_PORTION_SHIFT) * TEMPERATURE_FACTOR;
    int integer = data[TEMPERATURE_DATA_LSB_OFFSET] >> TEMPERATURE_DATA_INT_PORTION_SHIFT;
    integer = integer + data[TEMPERATURE_DATA_MSB_OFFSET];
    return integer + decimal;
#endif
}

int termopar_getStatus(void)
{
    int status = TERMOPAR_STATUS_OK;
#ifdef MAX6675
    return status;
#else
    char data[TEMPERATURE_DATA_SIZE];
    bool circuito_aberto = false;
    bool curto_vcc = false;
    bool curto_gnd = false;

    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, false);
    sleep_ms(1);
    spi_read_blocking(spi_default, 0, data, TEMPERATURE_DATA_SIZE);
    sleep_ms(1);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, true);
    uint16_t fault = ((data[1] & 0x01) << 8) | (data[3] & 0x07);
    if (fault)
    {
        log_message("Failure detected!");
        if (data[TEMPERATURE_DATA_ERROR_DATA_OFFSET] & TEMPERATURE_SENSOR_ERROR_OPEN_MASK)
        {
            log_message("[ERROR]: OPEN CIRCUIT - Termopar disconnected!");
            status = TERMOPAR_STATUS_OPEN;
        }
        if (data[TEMPERATURE_DATA_ERROR_DATA_OFFSET] & TEMPERATURE_SENSOR_ERROR_SC_GND_MASK)
        {
            log_message("[ERROR]: SHORT CIRCUIT to GND");
            status = TERMOPAR_STATUS_SC_GND;
        }
        if (data[TEMPERATURE_DATA_ERROR_DATA_OFFSET] & TEMPERATURE_SENSOR_ERROR_SC_VCC_MASK)
        {
            log_message("[ERROR]: SHORT CIRCUIT to VCC");
            status = TERMOPAR_STATUS_SC_VCC;
        }
    }
#endif
    return status;
}
