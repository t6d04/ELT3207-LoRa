// lora.c - Nhận dữ liệu LoRa cho Receiver (STM32F411RE)
#include "stm32f4xx.h"
#include "timer.h"
#include "led.h"
#include "lora.h"
#include "buzzer.h"
#include <string.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "i2c.h"
#include "uart.h"
#define LORA_NSS_LOW()   (GPIOA->BSRR = GPIO_BSRR_BR4)
#define LORA_NSS_HIGH()  (GPIOA->BSRR = GPIO_BSRR_BS4)
#define LORA_RST_LOW()   (GPIOA->BSRR = GPIO_BSRR_BR2)
#define LORA_RST_HIGH()  (GPIOA->BSRR = GPIO_BSRR_BS2)



static void spi1_write(uint8_t data) {
    while (!(SPI1->SR & SPI_SR_TXE));
    *(volatile uint8_t*)&SPI1->DR = data;
    while (!(SPI1->SR & SPI_SR_RXNE));
    (void)SPI1->DR;
}

static uint8_t spi1_transfer(uint8_t data) {
    while (!(SPI1->SR & SPI_SR_TXE));
    *(volatile uint8_t*)&SPI1->DR = data;
    while (!(SPI1->SR & SPI_SR_RXNE));
    return (uint8_t)SPI1->DR;
}

static void lora_write_reg(uint8_t addr, uint8_t value) {
    LORA_NSS_LOW();
    spi1_write(addr | 0x80);
    spi1_write(value);
    LORA_NSS_HIGH();
}

static uint8_t lora_read_reg(uint8_t addr) {
    LORA_NSS_LOW();
    spi1_write(addr & 0x7F);
    uint8_t val = spi1_transfer(0x00);
    LORA_NSS_HIGH();
    return val;
}

void SPI1_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // SPI1: PA5=SCK, PA6=MISO, PA7=MOSI
    GPIOA->MODER &= ~(GPIO_MODER_MODE5 | GPIO_MODER_MODE6 | GPIO_MODER_MODE7);
    GPIOA->MODER |=  (GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1);
    GPIOA->AFR[0] |= (5 << (5 * 4)) | (5 << (6 * 4)) | (5 << (7 * 4));

    // NSS (PA4) là output điều khiển thủ công
    GPIOA->MODER &= ~(GPIO_MODER_MODE4);
    GPIOA->MODER |=  (GPIO_MODER_MODE4_0);
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT4);

    SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_BR_1;
    SPI1->CR1 |= SPI_CR1_SPE;
}

void LORA_GPIO_Init(void) {
    // PA2: RST
    GPIOA->MODER &= ~(GPIO_MODER_MODE2);
    GPIOA->MODER |=  (GPIO_MODER_MODE2_0);
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT2);

    // PA0: DIO0 - input + EXTI0
    GPIOA->MODER &= ~(GPIO_MODER_MODE0);
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD0);

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
    EXTI->IMR  |= EXTI_IMR_MR0;
    EXTI->RTSR |= EXTI_RTSR_TR0;
    NVIC_EnableIRQ(EXTI0_IRQn);
}

void lora_init_rx(void) {
    led_on(LED_POWER_PORT, LED_POWER_PIN);
    LORA_RST_LOW();
    timer_delay_ms(10);
    LORA_RST_HIGH();
    timer_delay_ms(10);
    uint8_t count = 0;
    while (lora_read_reg(REG_VERSION) != 0x12 && count < 10) {
        led_on(LED_IGNORE_PORT, LED_IGNORE_PIN);
        timer_delay_ms(500);
        count++;
    }
    if (count >= 10) {
        GPIOC->ODR |= (1 << 3);
        return;
    }
    lora_write_reg(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
    lora_write_reg(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
    lora_write_reg(REG_FREQ_MSB, 0x6C);
    lora_write_reg(REG_FREQ_MID, 0x80);
    lora_write_reg(REG_FREQ_LSB, 0x00);
    lora_write_reg(REG_PREAMBLE_MSB, 0x00);
    lora_write_reg(REG_PREAMBLE_LSB, 0x08);
    lora_write_reg(REG_SYNC_WORD, 0x12);
    lora_write_reg(REG_MODEM_CONFIG1, 0x72);
    lora_write_reg(REG_MODEM_CONFIG2, 0xC0); // SF12
    lora_write_reg(REG_MODEM_CONFIG3, 0x04);
    lora_write_reg(REG_FIFO_RX_BASE_ADDR, 0x00);
    lora_write_reg(REG_DIO_MAPPING1, 0x00);
    lora_write_reg(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_CONTINUOUS);
    led_on(LED_OK_PORT, LED_OK_PIN);
}


void lora_handle_packet_interrupt(void) {
    char debug[128];
    uint8_t irq_flags = lora_read_reg(REG_IRQ_FLAGS);
    snprintf(debug, sizeof(debug), "IRQ Flags: 0x%02X\r\n", irq_flags);
    uart2_send_string(debug); // Gửi cờ IRQ qua UART

    if (irq_flags & 0x40) { // Kiểm tra RxDone
        uint8_t len = lora_read_reg(REG_RX_NB_BYTES);
        snprintf(debug, sizeof(debug), "Received %d bytes\r\n", len);
        uart2_send_string(debug);

        if (len > 32) {
            uart2_send_string("Error: Packet too long\r\n");
            return;
        }

        lora_write_reg(REG_FIFO_ADDR_PTR, lora_read_reg(REG_FIFO_RX_CURRENT));
        uint8_t buffer[32];

        LORA_NSS_LOW();
        spi1_write(REG_FIFO & 0x7F);
        for (uint8_t i = 0; i < len; i++) {
            buffer[i] = spi1_transfer(0x00);
        }
        LORA_NSS_HIGH();

        // Gửi dữ liệu thô qua UART
        uart2_send_string("Raw data: ");
        for (uint8_t i = 0; i < len; i++) {
            snprintf(debug, sizeof(debug), "0x%02X ", buffer[i]);
            uart2_send_string(debug);
        }
        uart2_send_string("\r\n");

        lora_write_reg(REG_IRQ_FLAGS, 0xFF); // Xóa cờ ngắt

        uint8_t crc = 0;
        for (uint8_t i = 0; i < len - 1; i++) {
            crc ^= buffer[i];
        }

        if (crc == buffer[len - 1]) {
            led_on(LED_RAW_PORT, LED_RAW_PIN);
            uint8_t dev_id = buffer[0];
            uint32_t timestamp = buffer[1] | (buffer[2] << 8) | (buffer[3] << 16) | (buffer[4] << 24);
            float lat, lon;
            memcpy(&lat, &buffer[5], 4);
            memcpy(&lon, &buffer[9], 4);

            snprintf(debug, sizeof(debug), "Valid packet: DevID=%d, Timestamp=%lu, Lat=%.6f, Lon=%.6f\r\n",
                     dev_id, timestamp, lat, lon);
            uart2_send_string(debug);

            char line1[17], line2[17];
            snprintf(line1, sizeof(line1), "Lat: %.6f", lat);
            snprintf(line2, sizeof(line2), "Lon: %.6f", lon);

            lcd_command(0x01); // Xóa LCD
            lcd_print(line1);  // Dòng 1
            lcd_command(0xC0); // Xuống dòng 2
            lcd_print(line2);  // Dòng 2

            buzzer_on();
        } else {
            led_on(LED_IGNORE_PORT, LED_IGNORE_PIN);
            snprintf(debug, sizeof(debug), "CRC Error: Calculated=0x%02X, Received=0x%02X\r\n",
                     crc, buffer[len - 1]);
            uart2_send_string(debug);
            lcd_command(0x01);
            lcd_print("CRC Loi");
        }
    } else {
        uart2_send_string("No RxDone\r\n");
        lcd_command(0x01);
        lcd_print("Khong RxDone");
        buzzer_off();
        lora_write_reg(REG_IRQ_FLAGS, 0xFF);
    }
}

