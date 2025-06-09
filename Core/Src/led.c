#include "stm32f4xx.h"
#include "led.h"
#include "timer.h"  // Cần có Timer_DelayMs()

void led_on(uint8_t pin) {
    GPIOB->ODR |= (1 << pin);
}

void led_off(uint8_t pin) {
    GPIOB->ODR &= ~(1 << pin);
}

void led_toggle(uint8_t pin) {
    GPIOB->ODR ^= (1 << pin);
}

void led_blink(uint8_t pin, int times, int delay_ms) {
    for (int i = 0; i < times; i++) {
        led_on(pin);
        timer_delayms(delay_ms);
        led_off(pin);
        timer_delayms(delay_ms);
    }
}
