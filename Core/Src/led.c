#include "stm32f4xx.h"
#include "led.h"
#include "timer.h"

static Led leds[] = {
    {LED_POWER_PORT, LED_POWER_PIN, 0, 0, 0},
    {LED_OK_PORT,    LED_OK_PIN,    0, 0, 0},
    {LED_RAW_PORT,   LED_RAW_PIN,   0, 0, 0},
    {LED_IGNORE_PORT,LED_IGNORE_PIN,0, 0, 0}
};

#define NUM_LEDS (sizeof(leds) / sizeof(leds[0]))

void led_on(GPIO_TypeDef* port, uint8_t pin) {
	for (int i = 0; i < NUM_LEDS; i++){
		if (leds[i].port == port && leds[i].pin == pin){
			if (!leds[i].active){
				leds[i].active = 1;
				port->ODR |= (1 << pin);
			}
			leds[i].start_time = millis();
		}
	}
}

void led_off(GPIO_TypeDef* port, uint8_t pin) {
	for (int i = 0; i < NUM_LEDS; i++){
		if (leds[i].port == port && leds[i].pin == pin){
			port->ODR &= ~(1 << pin);
			leds[i].active = 0;
		}
	}
}

void led_toggle(GPIO_TypeDef* port, uint8_t pin) {
	for (int i = 0; i < NUM_LEDS; i++){
		if (leds[i].port == port && leds[i].pin == pin){
			port->ODR ^= (1 << pin);
			if (leds[i].active == 0){
				leds[i].active = 1;
				leds[i].start_time = millis();
			}
			else {
				leds[i].active = 0;
			}
		}
	}
}

void led_blink(GPIO_TypeDef* port, uint8_t pin, int delay_ms) {
	led_on(port, pin);
	for (int i = 0; i < NUM_LEDS; i++){
		if (leds[i].port == port && leds[i].pin == pin){
			leds[i].duration = delay_ms;
		}
	}
}

void led_check(){
	for (int i = 0; i < NUM_LEDS; i++){
		if(leds[i].active && leds[i].duration && millis() - leds[i].start_time >= leds[i].duration){
			leds[i].duration = 0;
			led_off(leds[i].port, leds[i].pin);
		}
	}
}
