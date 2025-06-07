#ifndef LED_H
#define LED_H

// LED chân PB0 và PB1 tương ứng với LED1 = 0, LED2 = 1
#define LED1 0
#define LED2 1

void LED_On(int led);
void LED_Off(int led);
void LED_Toggle(int led);
void LED_Blink(int led, int times, int delay_ms);

#endif
