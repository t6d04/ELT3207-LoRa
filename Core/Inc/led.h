#ifndef LED_H
#define LED_H

void LED_Init(void);
void LED_On(int led);
void LED_Off(int led);
void LED_Toggle(int led);
void LED_Blink(int led, int times, int delay_ms);

#endif
