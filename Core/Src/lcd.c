#include "i2c.h"
#include "lcd.h"
#include "stm32f4xx.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define LCD_ADDR       0x27
#define LCD_BACKLIGHT  0x08
#define LCD_ENABLE     0x04
#define LCD_COMMAND    0
#define LCD_DATA       1

static void lcd_write_nibble(uint8_t nibble, uint8_t mode) {
    uint8_t data = (nibble & 0xF0) | LCD_BACKLIGHT | mode;

    // Send with EN=1 then EN=0 to latch
    I2C1_WriteByteTo(LCD_ADDR, data | LCD_ENABLE);
    for (volatile int i = 0; i < 2000; i++);
    I2C1_WriteByteTo(LCD_ADDR, data & ~LCD_ENABLE);
}

static void lcd_send(uint8_t value, uint8_t mode) {
    lcd_write_nibble(value & 0xF0, mode);
    lcd_write_nibble((value << 4) & 0xF0, mode);
}

static void lcd_send_cmd(uint8_t cmd) {
    lcd_send(cmd, LCD_COMMAND);
}

static void lcd_send_data(uint8_t data) {
    lcd_send(data, LCD_DATA);
}

void LCD_Init(void) {
    I2C1_Init();

    // Wait for LCD power-up
    for (volatile int i = 0; i < 800000; i++);

    // Init sequence (as per datasheet for 4-bit mode)
    lcd_write_nibble(0x30, LCD_COMMAND);
    for (volatile int i = 0; i < 80000; i++);
    lcd_write_nibble(0x30, LCD_COMMAND);
    for (volatile int i = 0; i < 80000; i++);
    lcd_write_nibble(0x30, LCD_COMMAND);
    for (volatile int i = 0; i < 80000; i++);
    lcd_write_nibble(0x20, LCD_COMMAND); // Set 4-bit mode

    // Function set: 4-bit, 2 line, 5x8 font
    lcd_send_cmd(0x28);
    // Display ON, Cursor OFF
    lcd_send_cmd(0x0C);
    // Clear display
    lcd_send_cmd(0x01);
    for (volatile int i = 0; i < 80000; i++);
    // Entry mode: Increment, no shift
    lcd_send_cmd(0x06);
}

void LCD_Clear(void) {
    lcd_send_cmd(0x01);
    for (volatile int i = 0; i < 80000; i++);
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? 0x00 : 0x40;
    lcd_send_cmd(0x80 | (addr + col));
}

void LCD_Print(char *str) {
    while (*str) {
        lcd_send_data(*str++);
    }
}
