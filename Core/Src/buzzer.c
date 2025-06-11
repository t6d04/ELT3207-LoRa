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

void buzzer_alert(int delay_ms) {
	buzzer_on();
	timer_delay_ms(delay_ms);
	buzzer_off();
}
