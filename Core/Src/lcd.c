#include "stm32f4xx.h"
#include "lcd.h"

void lcd_i2c_start(void) {
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));
    I2C1->DR = (LCD_ADDR << 1);               // Gửi địa chỉ LCD (ghi)
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR1; (void)I2C1->SR2;
}

void lcd_i2c_stop(void) {
    I2C1->CR1 |= I2C_CR1_STOP;
}

void lcd_i2c_write(uint8_t data) {
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = data;
    while (!(I2C1->SR1 & I2C_SR1_BTF));
}

void lcd_write_nibble(uint8_t nibble, uint8_t control) {
    uint8_t data = (nibble & 0xF0) | control | LCD_BACKLIGHT;
    lcd_i2c_start();
    lcd_i2c_write(data);
    lcd_i2c_write(data | LCD_ENABLE);
    for (volatile int i = 0; i < 1000; i++);
    lcd_i2c_write(data & ~LCD_ENABLE);
    lcd_i2c_stop();
}

void lcd_write_byte(uint8_t byte, uint8_t control) {
    lcd_write_nibble(byte & 0xF0, control);
    lcd_write_nibble((byte << 4) & 0xF0, control);
}

void lcd_command(uint8_t cmd) {
    lcd_write_byte(cmd, 0x00);
    for (volatile int i = 0; i < 5000; i++);
}

void lcd_data(uint8_t data) {
    lcd_write_byte(data, LCD_REGISTER_SELECT);
    for (volatile int i = 0; i < 5000; i++);
}

void lcd_init(void) {
    for (volatile int i = 0; i < 100000; i++);
    lcd_command(0x33);
    lcd_command(0x32);
    lcd_command(0x28);
    lcd_command(0x0C);
    lcd_command(0x06);
    lcd_command(0x01);
    for (volatile int i = 0; i < 50000; i++);
}

void lcd_print(char *str) {
    while (*str) {
        lcd_data((uint8_t)(*str++));
    }
}
