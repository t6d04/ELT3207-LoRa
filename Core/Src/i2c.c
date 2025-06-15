#include "stm32f4xx.h"
#include "i2c.h"

// I2C1: SCL = PB8, SDA = PB9
void i2c2_init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;    // Enable GPIOB clock
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;     // Enable I2C1 clock

    // PB8 - SCL
    GPIOB->MODER &= ~(3 << (8 * 2));
    GPIOB->MODER |=  (2 << (8 * 2));         // Alternate function mode
    GPIOB->AFR[1] &= ~(0xF << ((8 - 8) * 4));
    GPIOB->AFR[1] |=  (4 << ((8 - 8) * 4));   // AF4 for I2C1
    GPIOB->OTYPER |= (1 << 8);               // Open-drain
    GPIOB->PUPDR &= ~(3 << (8 * 2));
    GPIOB->PUPDR |=  (1 << (8 * 2));          // Pull-up

    // PB9 - SDA
    GPIOB->MODER &= ~(3 << (9 * 2));
    GPIOB->MODER |=  (2 << (9 * 2));         // Alternate function mode
    GPIOB->AFR[1] &= ~(0xF << ((9 - 8) * 4));
    GPIOB->AFR[1] |=  (4 << ((9 - 8) * 4));   // AF4 for I2C1
    GPIOB->OTYPER |= (1 << 9);               // Open-drain
    GPIOB->PUPDR &= ~(3 << (9 * 2));
    GPIOB->PUPDR |=  (1 << (9 * 2));          // Pull-up

    // I2C1 configuration (100kHz @ APB1 = 16 MHz)
    I2C1->CR2 = 16;              // APB1 frequency in MHz
    I2C1->CCR = 80;              // Clock control value
    I2C1->TRISE = 17;            // Maximum rise time
    I2C1->CR1 |= I2C_CR1_PE;     // Enable I2C1
}

uint8_t i2c2_check_address(uint8_t address) {
    I2C1->CR1 |= I2C_CR1_START;                          // Generate START
    while (!(I2C1->SR1 & I2C_SR1_SB));                   // Wait for SB = 1

    I2C1->DR = address;                                  // Send address

    while ((I2C1->SR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0); // Wait for response

    if (I2C1->SR1 & I2C_SR1_ADDR) {
        (void)I2C1->SR1;
        (void)I2C1->SR2;
        I2C1->CR1 |= I2C_CR1_STOP;
        return 1;                                        // Slave responded
    } else {
        I2C1->SR1 &= ~I2C_SR1_AF;                        // Clear AF flag
        I2C1->CR1 |= I2C_CR1_STOP;
        return 0;                                        // No response
    }
}
