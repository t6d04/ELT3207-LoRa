#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include "stm32f4xx.h"

// ==== Chân nút nhấn ====
#define BTN_ONOFF_PORT     GPIOC
#define BTN_ONOFF_PIN      13

#define BTN_RESET_PORT     GPIOB
#define BTN_RESET_PIN      3

#define BTN_BUZZ_PORT      GPIOB
#define BTN_BUZZ_PIN       5

// ==== Trạng thái hệ thống ====
extern volatile uint8_t system_powered_on;
extern volatile uint8_t buzzer_should_off;

// ==== Khởi tạo ====
void button_init(void);

#endif
