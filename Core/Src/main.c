#include "stm32f4xx.h"
#include "gpio.h"
#include "button.h"
#include "system.h"

int main(void)
{
	gpio_init_all();
	button_init();
//	GPIOB->ODR |= (1 << 0);
	while(1);
}
