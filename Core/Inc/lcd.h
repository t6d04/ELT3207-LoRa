#ifndef __LCD_H
#define __LCD_H

#include <stdint.h>

#define LCD_ADDR 0x3F
#define LCD_BACKLIGHT 0x08

void lcd_clear(void);
void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_init(void);
void lcd_put_cursor(int row, int col);
void lcd_send_string(const char *str);

#endif
