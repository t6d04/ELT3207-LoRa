#include "lora.h"
#include "stm32f4xx.h"
#include <string.h>

// Macro điều khiển NSS chân PA4
#define NSS_LOW()   (GPIOA->BSRR = (uint32_t)GPIO_BSRR_BR_4)
#define NSS_HIGH()  (GPIOA->BSRR = (uint32_t)GPIO_BSRR_BS_4)

static uint8_t spi_transfer(uint8_t data) {
    while (!(SPI1->SR & SPI_SR_TXE));
    *(volatile uint8_t *)&SPI1->DR = data;
    while (!(SPI1->SR & SPI_SR_RXNE));
    return (uint8_t)SPI1->DR;
}

void lora_write_reg(uint8_t addr, uint8_t val) {
    NSS_LOW();
    spi_transfer(addr | 0x80);
    spi_transfer(val);
    NSS_HIGH();
}

uint8_t lora_read_reg(uint8_t addr) {
    NSS_LOW();
    spi_transfer(addr & 0x7F);
    uint8_t val = spi_transfer(0x00);
    NSS_HIGH();
    return val;
}

static void lora_set_frequency(uint32_t freq) {
    uint32_t frf = (uint32_t)((double)freq / 61.03515625);
    lora_write_reg(REG_FRF_MSB, (frf >> 16) & 0xFF);
    lora_write_reg(REG_FRF_MID, (frf >> 8) & 0xFF);
    lora_write_reg(REG_FRF_LSB, frf & 0xFF);
}

void lora_init(void) {
    lora_write_reg(REG_OP_MODE, 0x80); // Sleep mode
    for (volatile int i = 0; i < 10000; i++);
    lora_write_reg(REG_OP_MODE, 0x81); // Standby

    lora_set_frequency(LORA_FREQ_HZ);
    lora_write_reg(REG_MODEM_CONFIG_1, 0x72); // BW=125kHz, CR=4/5, Explicit Header
    lora_write_reg(REG_MODEM_CONFIG_2, 0xC4); // SF12, CRC
    lora_write_reg(REG_PREAMBLE_MSB, LORA_PREAMBLE_MSB);
    lora_write_reg(REG_PREAMBLE_LSB, LORA_PREAMBLE_LSB);
    lora_write_reg(REG_SYNC_WORD, LORA_SYNCWORD);

    lora_write_reg(REG_FIFO_RX_BASE_ADDR, 0x00);
    lora_write_reg(REG_FIFO_ADDR_PTR, 0x00);
    lora_write_reg(REG_PAYLOAD_LENGTH, LORA_PAYLOAD_LENGTH);

    // Chuyển sang chế độ Rx liên tục
    lora_write_reg(REG_OP_MODE, 0x85); // LoRa RxContinuous mode
}

// ====== Hàm xử lý khi có interrupt từ DIO0 ======
void lora_handle_packet_interrupt(void) {
    if (!(lora_read_reg(REG_IRQ_FLAGS) & (1 << 6))) return;  // Không có gói mới

    uint8_t len = lora_read_reg(REG_RX_NB_BYTES);
    if (len != LORA_PAYLOAD_LENGTH) return;

    // Đọc dữ liệu từ FIFO
    lora_write_reg(REG_FIFO_ADDR_PTR, 0x00);
    uint8_t data[15];
    for (uint8_t i = 0; i < 15; i++) {
        data[i] = lora_read_reg(REG_FIFO);
    }

    // Xóa cờ IRQ
    lora_write_reg(REG_IRQ_FLAGS, 0xFF);

    // Phân tích data frame
    uint8_t  dev_id   = data[0];
    uint32_t ts       = (data[1]<<24) | (data[2]<<16) | (data[3]<<8) | data[4];
    float    lat;
    float    lon;
    memcpy(&lat, &data[5], 4);
    memcpy(&lon, &data[9], 4);
    uint8_t  checksum = data[13];

    // Tính lại checksum
    uint8_t sum = 0;
    for (int i = 0; i < 13; i++) sum ^= data[i];

    if (sum == checksum) {
        // TODO: Gọi LCD để hiển thị toạ độ
        // lcd_display(lat, lon);

        // TODO: Bật LED chỉ báo nhận thành công
        // led_on();

        // TODO: Nếu vượt rào ảo -> bật buzzer
        // buzzer_on();

    } else {
        // TODO: xử lý lỗi checksum (nếu cần)
    }
}
