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
//float lat =21.041578;
//float lon =105.793043;
int main(void) {

	gpio_init_all();
    i2c2_init();
    lcd_init();

    	timer_init();
    	button_init();
    	SPI1_Init();            // Khởi tạo SPI1 cho LoRa
    	LORA_GPIO_Init();       // Cấu hình chân GPIO LoRa (NSS, RST, DIO0)
    	lora_init_rx();

////
        while (1) {

        	led_check();
//
//        	lora_handle_packet_interrupt();

//        	buzzer_delay(100);
            //thêm if để xóa dùng button, 3 dong duoi la de xoa lcd
//           for (volatile int i = 0; i < 3000000; i++); // delay



}
}
