/*
 * timer.c
 *
 *  Created on: Jun 5, 2025
 *      Author: namlp
 */


#include "stm32f4xx.h"
#include "timer.h"

static volatile uint32_t ticks;

void timer_init(void) {
    // 1 ms tick (với HCLK = 16 MHz)
    SysTick->LOAD  = (16000 - 1);  // 16,000 cycles = 1 ms
    SysTick->VAL   = 0;
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_TICKINT_Msk   |
                     SysTick_CTRL_ENABLE_Msk;
}

void systick_handler(void) {
    if (ticks > 0) {
        ticks--;
    }
}

void timer_delayms(uint32_t ms) {
    ticks = ms;
    while (ticks > 0);
}
