#include "lcd.h"
#include "i2c.h"
#include "stm32f4xx.h"
#include <stdint.h>
#include <string.h>

#define LCD_ADDR 0x27
#define LCD_BACKLIGHT 0x08

#define ENABLE 0x04
#define RW     0x02
#define RS     0x01

static void lcd_send_nibble(uint8_t nibble, uint8_t control) {
    uint8_t data = (nibble & 0xF0) | control | LCD_BACKLIGHT;
    uint8_t data_enable = data | ENABLE;
    uint8_t data_disable = data & ~ENABLE;

    uint8_t packet[2] = { data_enable, data_disable };
    I2C1_WriteMulti(LCD_ADDR, packet, 2);
}

static void lcd_send_cmd(uint8_t cmd) {
    lcd_send_nibble(cmd & 0xF0, 0x00);
    lcd_send_nibble((cmd << 4) & 0xF0, 0x00);
}

static void lcd_send_data(uint8_t data) {
    lcd_send_nibble(data & 0xF0, RS);
    lcd_send_nibble((data << 4) & 0xF0, RS);
}

void LCD_Init(void) {
    I2C1_Init();

    // Wait >15ms after power-on
    for (volatile int i = 0; i < 200000; i++);

    lcd_send_nibble(0x30, 0x00);
    for (volatile int i = 0; i < 50000; i++);
    lcd_send_nibble(0x30, 0x00);
    for (volatile int i = 0; i < 50000; i++);
    lcd_send_nibble(0x30, 0x00);
    for (volatile int i = 0; i < 50000; i++);
    lcd_send_nibble(0x20, 0x00);  // 4-bit mode

    lcd_send_cmd(0x28);  // 4-bit, 2 line, 5x8 font
    lcd_send_cmd(0x0C);  // Display on, cursor off
    lcd_send_cmd(0x06);  // Entry mode: move right
    lcd_send_cmd(0x01);  // Clear display
    for (volatile int i = 0; i < 50000; i++);
}

void LCD_Clear(void) {
    lcd_send_cmd(0x01);
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? 0x00 + col : 0x40 + col;
    lcd_send_cmd(0x80 | addr);
}

void LCD_Print(char *str) {
    while (*str) {
        lcd_send_data((uint8_t)(*str++));
    }
}
