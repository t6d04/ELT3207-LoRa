#ifndef LED_H
#define LED_H

// Định nghĩa đúng chân GPIO theo bảng
#define LED1_PIN 0  // PB0
#define LED2_PIN 1  // PB1

void LED_On(uint8_t pin);
void LED_Off(uint8_t pin);
void LED_Toggle(uint8_t pin);
void LED_Blink(uint8_t pin, int times, int delay_ms);

#endif
