// ===================== i2c.c =====================
#include "stm32f4xx.h"
#include "i2c.h"
#include "timer.h"

void I2C1_Init(void) {
    // Thiết lập tốc độ truyền cho I2C1
    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->CR2 = 16;      // Tần số APB1 = 16 MHz
    I2C1->CCR = 80;      // 100kHz = 16MHz / (2 * 80)
    I2C1->TRISE = 17;    // TRISE = (1000ns / 62.5ns) + 1 = 17
    I2C1->CR1 |= I2C_CR1_PE; // Bật I2C
}

void i2c1_start(void) {
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB)); // Chờ cờ SB được set
    (void)I2C1->SR1; // Đọc để xóa cờ SB
}

void i2c1_stop(void) {
    I2C1->CR1 |= I2C_CR1_STOP;
    timer_delay_ms(1);
}

uint8_t i2c1_check_address(uint8_t address) {
    i2c1_start();
    I2C1->DR = address << 1; // Write mode

    // Chờ phản hồi
    while (!(I2C1->SR1 & (I2C_SR1_ADDR | I2C_SR1_AF)));

    if (I2C1->SR1 & I2C_SR1_ADDR) {
        (void)I2C1->SR1;
        (void)I2C1->SR2;
        i2c1_stop();
        return 1; // Có thiết bị
    } else {
        I2C1->SR1 &= ~I2C_SR1_AF; // Xóa lỗi ACK Fail nếu có
        i2c1_stop();
        return 0; // Không có thiết bị
    }
}

//void I2C1_WriteByte(uint8_t addr_7bit, uint8_t data) {
//    // 1. Wait until I2C bus is not busy
//    while (I2C1->SR2 & I2C_SR2_BUSY);
//
//    // 2. Send START
//    I2C1->CR1 |= I2C_CR1_START;
//    while (!(I2C1->SR1 & I2C_SR1_SB));
//    (void)I2C1->SR1;  // Clear SB by reading SR1
//
//    // 3. Send slave address
//    I2C1->DR = addr_7bit << 1;  // Write mode
//    while (!(I2C1->SR1 & I2C_SR1_ADDR));
//    (void)I2C1->SR2;  // Clear ADDR by reading SR2
//
//    // 4. Send data byte
//    while (!(I2C1->SR1 & I2C_SR1_TXE));
//    I2C1->DR = data;
//    while (!(I2C1->SR1 & I2C_SR1_BTF));
//
//    // 5. Send STOP
//    I2C1->CR1 |= I2C_CR1_STOP;
//}
