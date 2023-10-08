#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "lcd.h"

#define LCD_LIMPA_TELA 0x01
#define LCD_INICIA 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_FUNCTIONSET 0x20

#define LCD_INICIO_ESQUERDA 0x02

#define LCD_LIGA_DISPLAY 0x04

#define LCD_16x2 0x08

#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE_BIT 0x04

// By default these LCD display drivers are on bus address 0x27
#define BUS_ADDR 0x27

// Modes for lcd_envia_byte
#define LCD_CARACTER 1
#define LCD_COMANDO 0

#define MAX_LINES 2
#define MAX_CHARS 16

#define DELAY_US 600

void lcd_envia_comando(uint8_t val)
{
    i2c_write_blocking(i2c_default, BUS_ADDR, &val, 1, false);
}

void lcd_pulsa_enable(uint8_t val)
{
    sleep_us(DELAY_US);
    lcd_envia_comando(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    lcd_envia_comando(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

void lcd_envia_byte(uint8_t caractere, int modo)
{
    uint8_t nible_high = modo | (caractere & 0xF0) | LCD_BACKLIGHT;
    uint8_t nible_low = modo | ((caractere << 4) & 0xF0) | LCD_BACKLIGHT;

    lcd_envia_comando(nible_high);
    lcd_pulsa_enable(nible_high);
    lcd_envia_comando(nible_low);
    lcd_pulsa_enable(nible_low);
}

void lcd_clear(void)
{
    lcd_envia_byte(LCD_LIMPA_TELA, LCD_COMANDO);
}

void lcd_set_cursor(int linha, int coluna)
{
    int aux = (linha == 0) ? 0x80 + coluna : 0xC0 + coluna;
    lcd_envia_byte(aux, LCD_COMANDO);
}

void lcd_envia_caracter(char caractere)
{
    lcd_envia_byte(caractere, LCD_CARACTER);
}

void lcd_string(const char *s)
{
    while (*s)
    {
        lcd_envia_caracter(*s++);
    }
}

void lcd_init()
{ // This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
    // i2c_init(i2c_default, 100 * 1000);
    // gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    // gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    // gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    // gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    lcd_envia_byte(LCD_INICIA, LCD_COMANDO);
    lcd_envia_byte(LCD_INICIA | LCD_LIMPA_TELA, LCD_COMANDO);
    lcd_envia_byte(LCD_ENTRYMODESET | LCD_INICIO_ESQUERDA, LCD_COMANDO);
    lcd_envia_byte(LCD_FUNCTIONSET | LCD_16x2, LCD_COMANDO);
    lcd_envia_byte(LCD_DISPLAYCONTROL | LCD_LIGA_DISPLAY, LCD_COMANDO);
    lcd_clear();
}
