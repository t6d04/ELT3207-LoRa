#include "stm32f4xx.h"
#include "gpio.h"
#include "button.h"
#include "system.h"
#include "lora.h"

int main(void)
{
	gpio_init_all();
	button_init();
	lora_init();
	uint8_t version = lora_read_reg(0x42);
	if (version == 0x12) {
	    // OK: LED sáng
		GPIOB->ODR |= (1 << 0);
	} else {
	    // Lỗi: bật LED đỏ
		GPIOB->ODR |= (1 << 1);
	}
//	GPIOB->ODR |= (1 << 0);
	while(1);
}
