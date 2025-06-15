#include "stm32f4xx.h"
#include "uart.h"

void uart2_init(void) {
    // Bật clock cho USART2 và GPIOA
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Cấu hình PA2 (TX) và PA3 (RX) cho USART2 (Alternate Function)
    GPIOA->MODER &= ~((0x3 << (2 * 2)) | (0x3 << (3 * 2)));
    GPIOA->MODER |= (0x2 << (2 * 2)) | (0x2 << (3 * 2)); // AF mode
    GPIOA->AFR[0] &= ~((0xF << (2 * 4)) | (0xF << (3 * 4)));
    GPIOA->AFR[0] |= (0x7 << (2 * 4)) | (0x7 << (3 * 4)); // AF7 cho USART2

    // Cấu hình USART2: 115200 baud, 8-bit, no parity, 1 stop bit
    USART2->BRR = SystemCoreClock / 115200; // Với SystemCoreClock = 16 MHz
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; // Bật TX, RX, USART
}

void uart2_send_char(char c) {
    while (!(USART2->SR & USART_SR_TXE)); // Chờ TXE
    USART2->DR = c;
}

void uart2_send_string(const char* str) {
    while (*str) {
        uart2_send_char(*str++);
    }
}
