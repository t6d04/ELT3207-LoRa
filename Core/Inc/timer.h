// timer.h - Header cho module Timer hỗ trợ delay blocking và non-blocking
#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>

// Khởi tạo timer (SysTick)
void timer_init(void);

// Delay blocking (dừng CPU)
void timer_delay_ms(uint32_t ms);

// Thời gian đã trôi qua (non-blocking)
uint32_t millis(void);

// Reset thời gian (tùy chọn)
void timer_reset(void);

void timer_systick_callback(void);

#endif
