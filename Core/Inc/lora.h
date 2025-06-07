/*
 * lora.h
 *
 *  Created on: Jun 5, 2025
 *      Author: namlp
 */


#ifndef __LORA_H
#define __LORA_H

#include <stdint.h>
#include <stdbool.h>

#define LORA_FRAME_SIZE 14

typedef struct {
    uint8_t dev_id;
    uint32_t timestamp;
    float latitude;
    float longitude;
} GPS_Frame_t;

// Init UART2 để giao tiếp LoRa
void LORA_Init(void);

// ISR cho USART2
void USART2_IRQHandler(void);

// Parse và kiểm tra checksum
bool LORA_ParseFrame(const uint8_t *raw_data, GPS_Frame_t *frame);

#endif
