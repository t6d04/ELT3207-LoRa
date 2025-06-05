#include "stm32f4xx.h"

void delay(volatile uint32_t time)
{
    while (time--);
}

int main(void)
{
    // 1. Bật clock cho GPIOA (bit 0 của AHB1ENR)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // 2. Cấu hình PA5 là output (MODER5 = 01)
    GPIOA->MODER &= ~(0x3 << (5 * 2));      // Clear MODER5
    GPIOA->MODER |=  (0x1 << (5 * 2));      // Set MODER5 to 01 (output)

    // 3. Cấu hình output push-pull, không pull-up/pull-down (default rồi)

    while (1)
    {
        // Bật LED (PA5 = 1)
        GPIOA->ODR |= (1 << 5);
        delay(1000000);

        // Tắt LED (PA5 = 0)
        GPIOA->ODR &= ~(1 << 5);
        delay(1000000);
    }
}
