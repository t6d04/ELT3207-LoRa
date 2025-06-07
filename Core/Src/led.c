#include "stm32f4xx.h"
#include "led.h"
#include "timer.h"  // Giả định bạn đã có Timer_DelayMs()

void LED_On(int led) {
    GPIOB->ODR |= (1 << led);  // PB0 hoặc PB1
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
