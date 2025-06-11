// timer.c - Delay bằng TIM2 (không dùng SysTick)
#include "stm32f4xx.h"
#include "timer.h"

void timer_init(void) {
    // Bật clock cho TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Reset TIM2
    TIM2->CR1 = 0;
    TIM2->PSC = 16000 - 1;   // Chia tần số: 16MHz / 16000 = 1kHz (1ms mỗi tick)
    TIM2->ARR = 0xFFFF;      // Đếm tối đa
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;  // Bắt đầu đếm
}

void timer_delay_ms(uint32_t ms) {
    uint16_t start = TIM2->CNT;
    while ((uint16_t)(TIM2->CNT - start) < ms);
}

uint32_t timer_is_busy(void) {
    return 0; // Không dùng trong phiên bản này
}
