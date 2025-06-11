#include "stm32f4xx.h"
#include "led.h"
#include "timer.h"  // Cần có Timer_DelayMs()

void led_on(GPIO_TypeDef* port, uint8_t pin) {
    port->ODR |= (1 << pin);
}

void led_off(GPIO_TypeDef* port, uint8_t pin) {
    port->ODR &= ~(1 << pin);
}

void led_toggle(GPIO_TypeDef* port, uint8_t pin) {
    port->ODR ^= (1 << pin);
}

void led_blink(GPIO_TypeDef* port, uint8_t pin, int times, int delay_ms) {
    for (int i = 0; i < times; i++) {
        led_on(port, pin);
        timer_delay_ms(delay_ms);
        led_off(port, pin);
        timer_delay_ms(delay_ms);
    }
}
