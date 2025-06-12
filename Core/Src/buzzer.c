#include "stm32f4xx.h"
#include "buzzer.h"
#include "timer.h"

#define BUZZER_PIN 10  // PB10

static uint32_t buzzer_start = 0;
static uint32_t buzzer_running = 0;

void buzzer_on(void) {
	if (!buzzer_running){
		GPIOB->ODR |= (1 << BUZZER_PIN);
		buzzer_start = millis();
		buzzer_running = 1;
	}
}

void buzzer_off(void) {
    GPIOB->ODR &= ~(1 << BUZZER_PIN);
    buzzer_running = 0;
}

void buzzer_delay(int delay_ms) {
	if (millis() - buzzer_start >= delay_ms && buzzer_running == 1){
		buzzer_off();
	}
}
