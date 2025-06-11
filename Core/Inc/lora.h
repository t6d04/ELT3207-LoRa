// lora.h - Header cho module LoRa Receiver (STM32F411RE)
#ifndef __LORA_H
#define __LORA_H

#include "stm32f4xx.h"

// Định nghĩa các thanh ghi SX1278 cần dùng
#define REG_FIFO              0x00
#define REG_OP_MODE           0x01
#define REG_FIFO_ADDR_PTR     0x0D
#define REG_FIFO_RX_CURRENT   0x10
#define REG_IRQ_FLAGS         0x12
#define REG_RX_NB_BYTES       0x13
#define REG_PKT_RSSI_VALUE    0x1A
#define REG_MODEM_CONFIG1     0x1D
#define REG_MODEM_CONFIG2     0x1E
#define REG_MODEM_CONFIG3     0x26
#define REG_PREAMBLE_MSB      0x20
#define REG_PREAMBLE_LSB      0x21
#define REG_SYNC_WORD         0x39
#define REG_FIFO_RX_BASE_ADDR 0x0F
#define REG_DIO_MAPPING1      0x40
#define REG_VERSION           0x42
#define REG_FREQ_MSB          0x06
#define REG_FREQ_MID          0x07
#define REG_FREQ_LSB          0x08

#define MODE_LONG_RANGE_MODE  0x80
#define MODE_SLEEP            0x00
#define MODE_STDBY            0x01
#define MODE_RX_CONTINUOUS    0x05

// Khởi tạo SPI1 và GPIO liên quan đến LoRa
void SPI1_Init(void);
void LORA_GPIO_Init(void);

// Khởi tạo module LoRa để nhận
void lora_init_rx(void);

// Xử lý khi nhận gói tin (gọi từ EXTI0_IRQHandler)
void lora_handle_packet_interrupt(void);

#endif
