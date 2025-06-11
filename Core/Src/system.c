#include "stm32f4xx.h"
#include "system.h"
#include "gpio.h"
#include "lora.h"
#include "lcd.h"

// TODO: bổ sung các header nếu bạn có led.h, buzzer.h...

void system_startup(void) {
    gpio_init_all();
    // TODO: spi_init(), i2c_init() nếu chưa init
//    lcd_init();
//    lcd_clear();
//    lcd_print("Receiver DA BAT");
    // TODO: led_on();
}

void system_shutdown(void) {
    // Đưa LoRa về sleep mode

    // TODO: lcd_clear();
    // TODO: lcd_off(); nếu có

    // TODO: led_off(LED_GREEN); // Tắt LED trạng thái
    // TODO: buzzer_off();       // Tắt buzzer

    // Tắt clock ngoại vi không cần thiết
    RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN;
    RCC->APB1ENR &= ~RCC_APB1ENR_I2C1EN;

    // Đưa các GPIO output không dùng về analog để giảm rò rỉ dòng
    // LED (PB0, PB1), buzzer (PB10)
    GPIOB->MODER &= ~((0x3 << (0 * 2)) | (0x3 << (1 * 2)) | (0x3 << (10 * 2)));
    GPIOB->MODER |=  ((0x3 << (0 * 2)) | (0x3 << (1 * 2)) | (0x3 << (10 * 2))); // Analog mode

}
