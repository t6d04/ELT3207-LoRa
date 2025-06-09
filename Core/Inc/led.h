#ifndef LED_H
#define LED_H

// Định nghĩa đúng chân GPIO theo bảng
#define LED1_PIN 0  // PB0
#define LED2_PIN 1  // PB1

void led_on(uint8_t pin);
void led_off(uint8_t pin);
void led_toggle(uint8_t pin);
void led_blink(uint8_t pin, int times, int delay_ms);

#endif
