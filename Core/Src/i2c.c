#include "stm32f4xx.h"
#include "i2c.h"

// I2C2: SCL = PB10, SDA = PB3
void i2c2_init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    // PB10 - SCL
    GPIOB->MODER &= ~(3 << (10 * 2));
    GPIOB->MODER |=  (2 << (10 * 2));
    GPIOB->AFR[1] &= ~(0xF << ((10 - 8) * 4));
    GPIOB->AFR[1] |=  (4 << ((10 - 8) * 4));
    GPIOB->OTYPER |= (1 << 10);
    GPIOB->PUPDR &= ~(3 << (10 * 2));
    GPIOB->PUPDR |=  (1 << (10 * 2));

    // PB3 - SDA
    GPIOB->MODER &= ~(3 << (3 * 2));
    GPIOB->MODER |=  (2 << (3 * 2));
    GPIOB->AFR[0] &= ~(0xF << (3 * 4));
    GPIOB->AFR[0] |=  (9 << (3 * 4));  // AF9 for I2C2_SDA
    GPIOB->OTYPER |= (1 << 3);
    GPIOB->PUPDR &= ~(3 << (3 * 2));
    GPIOB->PUPDR |=  (1 << (3 * 2));

    // I2C cấu hình
    I2C2->CR2 = 16;
    I2C2->CCR = 80;
    I2C2->TRISE = 17;
    I2C2->CR1 |= I2C_CR1_PE;
}
uint8_t i2c2_check_address(uint8_t address) {
    I2C2->CR1 |= I2C_CR1_START;                          // Gửi tín hiệu START
    while (!(I2C2->SR1 & I2C_SR1_SB));                   // Chờ cờ SB được set

    I2C2->DR = address;                                  // Gửi địa chỉ slave

    // Chờ phản hồi ACK hoặc NACK
    while ((I2C2->SR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0);

    if (I2C2->SR1 & I2C_SR1_ADDR) {
        (void)I2C2->SR1;
        (void)I2C2->SR2;
        I2C2->CR1 |= I2C_CR1_STOP;
        return 1;                                        // Tìm thấy slave
    } else {
        I2C2->SR1 &= ~I2C_SR1_AF;                        // Xóa cờ lỗi
        I2C2->CR1 |= I2C_CR1_STOP;
        return 0;                                        // Không có phản hồi
    }
}
