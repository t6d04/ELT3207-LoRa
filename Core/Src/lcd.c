
#include "stm32f4xx.h"
#include "i2c.h"
#include "timer.h"
#include <stdint.h>

#define SLAVE_ADDRESS_LCD 0x4E

#define LCD_CMD_FUNCTION_SET     0x28
#define LCD_CMD_DISPLAY_OFF      0x08
#define LCD_CMD_DISPLAY_ON       0x0C
#define LCD_CMD_CLEAR_DISPLAY    0x01
#define LCD_CMD_ENTRY_MODE_SET   0x06
#define LCD_CMD_SET_CURSOR       0x80

static void lcd_write_4bit(uint8_t data);
void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_init(void);
void lcd_clear(void);
void lcd_put_cursor(int row, int col);
void lcd_send_string(const char *str);

static void lcd_write_4bit(uint8_t data) {
    // Gửi 1 byte tới I2C LCD
    I2C1_WriteByte(SLAVE_ADDRESS_LCD, data);
    timer_delayms(1);
}

void lcd_send_cmd(uint8_t cmd) {
    uint8_t upper = cmd & 0xF0;
    uint8_t lower = (cmd << 4) & 0xF0;

    lcd_write_4bit(upper | 0x0C);  // EN=1, RS=0
    lcd_write_4bit(upper | 0x08);  // EN=0
    lcd_write_4bit(lower | 0x0C);  // EN=1, RS=0
    lcd_write_4bit(lower | 0x08);  // EN=0
}

void lcd_send_data(uint8_t data) {
    uint8_t upper = data & 0xF0;
    uint8_t lower = (data << 4) & 0xF0;

    lcd_write_4bit(upper | 0x0D);  // EN=1, RS=1
    lcd_write_4bit(upper | 0x09);  // EN=0
    lcd_write_4bit(lower | 0x0D);  // EN=1, RS=1
    lcd_write_4bit(lower | 0x09);  // EN=0
}

void lcd_clear(void) {
    lcd_send_cmd(LCD_CMD_CLEAR_DISPLAY);
    timer_delayms(2);
}

void lcd_put_cursor(int row, int col) {
    uint8_t pos = (row == 0) ? (LCD_CMD_SET_CURSOR + col)
                             : (LCD_CMD_SET_CURSOR + 0x40 + col);
    lcd_send_cmd(pos);
}

void lcd_send_string(const char *str) {
    while (*str) {
        lcd_send_data((uint8_t)(*str++));
    }
}

void lcd_init(void) {
    timer_delayms(50); // >40ms sau khi bật nguồn

    lcd_write_4bit(0x30);
    timer_delayms(5);
    lcd_write_4bit(0x30);
    timer_delayms(1);
    lcd_write_4bit(0x30);
    timer_delayms(10);
    lcd_write_4bit(0x20); // 4-bit mode
    timer_delayms(10);

    lcd_send_cmd(LCD_CMD_FUNCTION_SET);
    lcd_send_cmd(LCD_CMD_DISPLAY_OFF);
    lcd_send_cmd(LCD_CMD_CLEAR_DISPLAY);
    lcd_send_cmd(LCD_CMD_ENTRY_MODE_SET);
    lcd_send_cmd(LCD_CMD_DISPLAY_ON);
}
