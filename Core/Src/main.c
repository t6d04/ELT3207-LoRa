#include "stm32f4xx.h"
#include "gpio.h"
#include "button.h"
#include "system.h"
#include "lora.h"
#include "lcd.h"
#include "led.h"
#include "i2c.h"
#include "main.h"
#include "timer.h"

int main(void)
{
	gpio_init_all();
	timer_init();
	button_init();
	SPI1_Init();            // Khởi tạo SPI1 cho LoRa
	LORA_GPIO_Init();       // Cấu hình chân GPIO LoRa (NSS, RST, DIO0)
	lora_init_rx();
	while (1) {
//		led_on(LED_OK_PORT, LED_OK_PIN);
	}



}
