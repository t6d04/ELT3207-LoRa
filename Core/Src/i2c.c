#include "stm32f4xx.h"
#include "i2c.h"

void i2c1_init(void) {
    // Enable I2C1 clock
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // Reset I2C1
    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->CR2 = 16;        // APB1 at 16MHz
    I2C1->CCR = 80;        // 100kHz = 16MHz / (2*CCR) => CCR = 80
    I2C1->TRISE = 17;      // TRISE = (1000ns / 62.5ns) + 1 = 17

    I2C1->CR1 |= I2C_CR1_PE;
}

void i2c1_start(void) {
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));
}

void i2c1_stop(void) {
    I2C1->CR1 |= I2C_CR1_STOP;
}

void i2c1_write(uint8_t data) {
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = data;
    while (!(I2C1->SR1 & I2C_SR1_BTF));
}

void i2c1_writebyteto(uint8_t addr, uint8_t data) {
    i2c1_start();
    I2C1->DR = addr << 1; // write mode
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;
    i2c1_write(data);
    i2c1_stop();
}
