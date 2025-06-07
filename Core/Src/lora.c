/*
 * lora.c
 *
 *  Created on: Jun 5, 2025
 *      Author: namlp
 */


#include "lora.h"
#include "stm32f4xx.h"

static uint8_t lora_rx_buffer[LORA_FRAME_SIZE];
static uint8_t lora_rx_index = 0;

void LORA_Init(void) {
    // Bật clock cho GPIOA và USART2
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // PA2 -> USART2_TX, PA3 -> USART2_RX
    GPIOA->MODER &= ~((3 << (2 * 2)) | (3 << (3 * 2))); // Clear mode
    GPIOA->MODER |= (2 << (2 * 2)) | (2 << (3 * 2));    // Alternate function
    GPIOA->AFR[0] |= (7 << (4 * 2)) | (7 << (4 * 3));   // AF7 for USART2

    // Cấu hình USART2: 9600 baud (ví dụ với 16MHz clock)
    USART2->BRR = 16000000 / 9600;
    USART2->CR1 |= USART_CR1_RE | USART_CR1_TE;        // Enable RX, TX
    USART2->CR1 |= USART_CR1_RXNEIE;                   // Enable RX interrupt
    USART2->CR1 |= USART_CR1_UE;                       // Enable USART

    NVIC_EnableIRQ(USART2_IRQn);
}

// ===== Interrupt handler cho UART2 RX =====
void USART2_IRQHandler(void) {
    if (USART2->SR & USART_SR_RXNE) {
        uint8_t byte = (uint8_t)(USART2->DR & 0xFF);
        lora_rx_buffer[lora_rx_index++] = byte;

        if (lora_rx_index >= LORA_FRAME_SIZE) {
            GPS_Frame_t frame;
            if (LORA_ParseFrame(lora_rx_buffer, &frame)) {
                // TODO: Xử lý frame hợp lệ tại đây
                // Ví dụ: check vượt rào, gọi LCD/LED
                // Ex: check_fence(frame.latitude, frame.longitude);
            }
            lora_rx_index = 0; // Reset cho frame tiếp theo
        }
    }
}

// ===== Parse và kiểm tra checksum =====
bool LORA_ParseFrame(const uint8_t *raw_data, GPS_Frame_t *frame) {
    uint8_t checksum = 0;
    for (int i = 0; i < LORA_FRAME_SIZE - 1; ++i) {
        checksum ^= raw_data[i];
    }

    if (checksum != raw_data[LORA_FRAME_SIZE - 1]) return false;

    // Parse fields
    frame->dev_id = raw_data[0];
    frame->timestamp = *((uint32_t*)&raw_data[1]);
    frame->latitude = *((float*)&raw_data[5]);
    frame->longitude = *((float*)&raw_data[9]);

    return true;
}
