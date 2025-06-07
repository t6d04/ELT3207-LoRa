#include "stm32f4xx.h"
#include "led.h"
#include "timer.h"  // Giả định bạn có timer_delay_ms()

#define LED1_PIN 0   // PB0
#define LED2_PIN 1   // PB1

void LED_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    GPIOB->MODER &= ~((0x3 << (LED1_PIN * 2)) | (0x3 << (LED2_PIN * 2)));
    GPIOB->MODER |=  ((0x1 << (LED1_PIN * 2)) | (0x1 << (LED2_PIN * 2)));

    GPIOB->OTYPER &= ~((1 << LED1_PIN) | (1 << LED2_PIN));
    GPIOB->OSPEEDR |= (0x3 << (LED1_PIN * 2)) | (0x3 << (LED2_PIN * 2));
}

void LED_On(int led) {
    GPIOB->ODR |= (1 << led);
}

void LED_Off(int led) {
    GPIOB->ODR &= ~(1 << led);
}

void LED_Toggle(int led) {
    GPIOB->ODR ^= (1 << led);
}

void LED_Blink(int led, int times, int delay_ms) {
    for (int i = 0; i < times; i++) {
        LED_On(led);
        Timer_DelayMs(delay_ms);
        LED_Off(led);
        Timer_DelayMs(delay_ms);
    }
}
