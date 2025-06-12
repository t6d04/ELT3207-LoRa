#ifndef __LCD_H__
#define __LCD_H__
#define LCD_ADDR 0x27
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE 0x04
#define LCD_REGISTER_SELECT 0x01
void lcd_i2c_start(void) ;
void lcd_i2c_stop(void);
void lcd_i2c_write(uint8_t data);
void lcd_write_nibble(uint8_t nibble, uint8_t control);
void lcd_write_byte(uint8_t byte, uint8_t control);
void lcd_command(uint8_t cmd) ;
void lcd_data(uint8_t data);
void lcd_init(void);
void lcd_print(char *str);

#endif
