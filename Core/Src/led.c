#include "stm32f4xx.h"
#include "led.h"
#include "timer.h"  // Cần có Timer_DelayMs()

void LED_On(uint8_t pin) {
    GPIOB->ODR |= (1 << pin);
}

void LED_Off(uint8_t pin) {
    GPIOB->ODR &= ~(1 << pin);
}

void LED_Toggle(uint8_t pin) {
    GPIOB->ODR ^= (1 << pin);
}

void LED_Blink(uint8_t pin, int times, int delay_ms) {
    for (int i = 0; i < times; i++) {
        LED_On(pin);
        Timer_DelayMs(delay_ms);
        LED_Off(pin);
        Timer_DelayMs(delay_ms);
    }
}
