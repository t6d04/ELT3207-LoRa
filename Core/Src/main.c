#include "stm32f4xx.h"
#include "gpio.h"
#include "i2c.h"
#include "lcd_i2c.h"

void delay(volatile uint32_t time)
{
    while (time--);
}

int main(void)
{
    // === 1. Khởi tạo I2C và LCD ===
    I2C1_Init();         // Hàm trong i2c.c: khởi tạo thanh ghi I2C
    lcd_init();          // Hàm trong lcd_i2c.c: khởi tạo LCD

    // === 2. Khởi tạo LED tại PA5  ===
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER &= ~(0x3 << (5 * 2));
    GPIOA->MODER |=  (0x1 << (5 * 2));

    // === 3. In nội dung ra LCD ===
    lcd_set_cursor(0, 0);                     // Dòng 0, cột 0
    lcd_send_string("STM32 + LCD I2C");

    lcd_set_cursor(1, 0);                     // Dòng 1
    lcd_send_string("Hello World!");

    // === 4. Nháy LED ===
    while (1)
    {
        GPIOA->ODR |= (1 << 5);
        delay(1000000);

        GPIOA->ODR &= ~(1 << 5);
        delay(1000000);
    }
}
