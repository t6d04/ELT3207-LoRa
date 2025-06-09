#include "stm32f4xx.h"
#include "buzzer.h"
#include "timer.h"  // Timer_DelayMs()

#define BUZZER_PIN 10  // PB10

void buzzer_on(void) {
    GPIOB->ODR |= (1 << BUZZER_PIN);
}

void buzzer_off(void) {
    GPIOB->ODR &= ~(1 << BUZZER_PIN);
}

void buzzer_alert(int times, int delay_ms) {
    for (int i = 0; i < times; i++) {
        buzzer_on();
        timer_delayms(delay_ms);
        buzzer_off();
        timer_delayms(delay_ms);
    }
}
