// timer.h - Header cho module delay bằng SysTick STM32F4
#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>

void timer_init(void);
void timer_delay_ms(uint32_t ms);
uint32_t timer_is_busy(void);

#endif // __TIMER_H
