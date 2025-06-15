#include "stm32f4xx.h"

void uart2_init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    GPIOA->MODER &= ~(0x3 << (2 * 2));
    GPIOA->MODER |= (0x2 << (2 * 2));
    GPIOA->AFR[0] |= (0x7 << (2 * 4)); // AF7 cho PA2 (TX)

    GPIOA->MODER &= ~(0x3 << (3 * 2));
    GPIOA->MODER |= (0x2 << (3 * 2));
    GPIOA->AFR[0] |= (0x7 << (3 * 4)); // AF7 cho PA3 (RX)

    USART2->BRR = SystemCoreClock / 115200; // Dùng SystemCoreClock
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE; // Bật TX, RX
    USART2->CR1 |= USART_CR1_UE; // Bật USART
}

void uart2_send_char(char c) {
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = c;
}

void uart2_send_string(const char* str) {
    while (*str) {
        uart2_send_char(*str++);
    }
}
