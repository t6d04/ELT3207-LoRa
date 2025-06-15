#include "stm32f4xx.h"
#include "lcd.h"
#include "i2c.h"
#include <string.h>
#include <stdio.h>
#include "gpio.h"
#include "button.h"
#include "system.h"
#include "lora.h"
#include "led.h"
#include "main.h"
#include "timer.h"
#include "buzzer.h"
#include "uart.h"

int main(void) {
    char debug[128];

    uart2_init(); // Khởi tạo UART đầu tiên
    snprintf(debug, sizeof(debug), "UART2 initialized\r\n");
    uart2_send_string(debug);

    gpio_init_all();
    snprintf(debug, sizeof(debug), "GPIO initialized\r\n");
    uart2_send_string(debug);

    i2c2_init(); // Đổi thành i2c1_init() nếu khớp với i2c.c
    snprintf(debug, sizeof(debug), "I2C initialized\r\n");
    uart2_send_string(debug);

    lcd_init();
    snprintf(debug, sizeof(debug), "LCD initialized\r\n");
    uart2_send_string(debug);

    timer_init();
    snprintf(debug, sizeof(debug), "Timer initialized\r\n");
    uart2_send_string(debug);

    button_init();
    snprintf(debug, sizeof(debug), "Buttons initialized\r\n");
    uart2_send_string(debug);

    SPI1_Init();
    snprintf(debug, sizeof(debug), "SPI1 initialized\r\n");
    uart2_send_string(debug);

    LORA_GPIO_Init();
    snprintf(debug, sizeof(debug), "LoRa GPIO initialized\r\n");
    uart2_send_string(debug);

    lora_init_rx();
    snprintf(debug, sizeof(debug), "LoRa RX initialized\r\n");
    uart2_send_string(debug);

    while (1) {
        led_check();
        snprintf(debug, sizeof(debug), "LED check loop running\r\n");
        uart2_send_string(debug);
        for (volatile int i = 0; i < 1000000; i++);
    }
}
