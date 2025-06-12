//#include "stm32f4xx.h"
//#include "gpio.h"
//#include "button.h"
//#include "system.h"
//#include "lora.h"
//#include "lcd.h"
//#include "led.h"
//#include "i2c.h"
//#include "main.h"
//#include "timer.h"
//#include "buzzer.h"
//
//#include "lcd.h"
//#include "string.h"
//#include "stdio.h"
//
//int main(void)
//{
//	gpio_init_all();
//	timer_init();
//	button_init();
//	SPI1_Init();            // Khởi tạo SPI1 cho LoRa
//	LORA_GPIO_Init();       // Cấu hình chân GPIO LoRa (NSS, RST, DIO0)
//	lora_init_rx();
//	i2c2_init();
//	lcd_init();
//	while (1) {
////		led_on(LED_OK_PORT, LED_OK_PIN);
//		lcd_command(0x01);
//		lcd_print("khon con");
////		led_check();
////		buzzer_delay(1000);
//		for(volatile int i = 0;i<1000000;i++);
//	}
//}

// ở trên là chương trình cũ trước khi duy sửa

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

int main(void) {
	gpio_init_all();
    i2c2_init();
    lcd_init();

    	timer_init();
//    	button_init();
    	SPI1_Init();            // Khởi tạo SPI1 cho LoRa
    	LORA_GPIO_Init();       // Cấu hình chân GPIO LoRa (NSS, RST, DIO0)
    	lora_init_rx();

        while (1) {

        			led_check();
        			buzzer_delay(1000);

            lcd_command(0x01); // xóa LCD cũ
            lcd_print("Khong co  !");
            lcd_command(0xC0); // dòng thứ 2
            lcd_print("alooo");

            //thêm if để xóa dùng button, 3 dong duoi la de xoa lcd
            for (volatile int i = 0; i < 3000000; i++); // delay
            lcd_command(0x01);    // Xóa màn hình


            for (volatile int i = 0; i < 3000000; i++); // delay

            // hàm bật tắt led: hiện 3 giây rồi tắt led rồi bật led, ( sau dùng button thì if sau)
//            for (volatile int i = 0; i < 3000000; i++); // delay
//            //            lcd_command(0x01);    // Xóa màn hình
//                        lcd_command(0x08);  // Display OFF, Cursor OFF, Blink OFF
//
//                        for (volatile int i = 0; i < 3000000; i++); // delay
//
//                        lcd_command(0x0C);

        }
}
