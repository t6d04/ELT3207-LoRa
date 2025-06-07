#include "stm32f4xx.h"
#include "system.h"
#include "gpio.h"
#include "lora.h"
#include "lcd.h"

// TODO: bổ sung các header nếu bạn có led.h, buzzer.h...

void system_startup(void) {
    gpio_init_all();
    // TODO: spi_init(), i2c_init() nếu chưa init

    lora_init();
//    lcd_init();
//    lcd_clear();
//    lcd_print("Receiver DA BAT");
    // TODO: led_on();
}

void system_shutdown(void) {
    lora_write_reg(REG_OP_MODE, 0x80); // Sleep mode


    // TODO: add led, lcd and buzzer

    // Dừng ngoại vi nếu tiết kiệm điện
    RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN;
    RCC->APB1ENR &= ~RCC_APB1ENR_I2C1EN;
}
