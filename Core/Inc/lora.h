#ifndef __LORA_H
#define __LORA_H

#include <stdint.h>

// ============ Định nghĩa thanh ghi SX1278 ============
#define REG_FIFO                0x00
#define REG_OP_MODE             0x01
#define REG_FRF_MSB             0x06
#define REG_FRF_MID             0x07
#define REG_FRF_LSB             0x08
#define REG_PA_CONFIG           0x09
#define REG_LNA                 0x0C
#define REG_FIFO_ADDR_PTR       0x0D
#define REG_FIFO_RX_BASE_ADDR   0x0F
#define REG_IRQ_FLAGS           0x12
#define REG_RX_NB_BYTES         0x13
#define REG_PKT_SNR_VALUE       0x19
#define REG_PKT_RSSI_VALUE      0x1A
#define REG_MODEM_CONFIG_1      0x1D
#define REG_MODEM_CONFIG_2      0x1E
#define REG_PREAMBLE_MSB        0x20
#define REG_PREAMBLE_LSB        0x21
#define REG_PAYLOAD_LENGTH      0x22
#define REG_MODEM_CONFIG_3      0x26
#define REG_SYNC_WORD           0x39
#define REG_VERSION             0x42

// ============ Cấu hình mặc định ============
#define LORA_FREQ_HZ            433000000UL
#define LORA_SF                 12
#define LORA_PREAMBLE_MSB       0x00
#define LORA_PREAMBLE_LSB       0x08
#define LORA_SYNCWORD           0x12
#define LORA_PAYLOAD_LENGTH     15

// ============ API ============
void lora_init(void);
void lora_handle_packet_interrupt(void);  // Gọi trong EXTI0_IRQHandler

uint8_t lora_read_reg(uint8_t addr);
void lora_write_reg(uint8_t addr, uint8_t val);

#endif
