#include "stm32f4xx.h"
#include "buzzer.h"
#include "timer.h"

#define BUZZER_PIN 10  // PB10

void Buzzer_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    GPIOB->MODER &= ~(0x3 << (BUZZER_PIN * 2));
    GPIOB->MODER |=  (0x1 << (BUZZER_PIN * 2)); // Output
    GPIOB->OTYPER &= ~(1 << BUZZER_PIN);
    GPIOB->OSPEEDR |= (0x3 << (BUZZER_PIN * 2));
}

void Buzzer_On(void) {
    GPIOB->ODR |= (1 << BUZZER_PIN);
}

void Buzzer_Off(void) {
    GPIOB->ODR &= ~(1 << BUZZER_PIN);
}

void Buzzer_Alert(int times, int delay_ms) {
    for (int i = 0; i < times; i++) {
        Buzzer_On();
        Timer_DelayMs(delay_ms);
        Buzzer_Off();
        Timer_DelayMs(delay_ms);
    }
}
