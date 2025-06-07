#include "stm32f4xx.h"
#include "i2c.h"

void I2C1_Init(void) {
    // Enable I2C1 clock
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // Reset I2C1
    RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;

    // Configure timing register (100kHz @ 16MHz APB1)
    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->CR2 = 16; // PCLK1 = 16 MHz
    I2C1->CCR = 80; // 100kHz
    I2C1->TRISE = 17;
    I2C1->CR1 |= I2C_CR1_PE;
}

void I2C1_Start(void) {
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));
}

void I2C1_Stop(void) {
    I2C1->CR1 |= I2C_CR1_STOP;
}

void I2C1_Write(uint8_t addr, uint8_t data) {
    I2C1_Start();
    I2C1->DR = addr << 1;  // Write
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;

    while (!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = data;

    while (!(I2C1->SR1 & I2C_SR1_BTF));
    I2C1_Stop();
}

void I2C1_WriteMulti(uint8_t addr, uint8_t *data, uint8_t size) {
    I2C1_Start();
    I2C1->DR = addr << 1;  // Write
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;

    for (uint8_t i = 0; i < size; i++) {
        while (!(I2C1->SR1 & I2C_SR1_TXE));
        I2C1->DR = data[i];
    }

    while (!(I2C1->SR1 & I2C_SR1_BTF));
    I2C1_Stop();
}
