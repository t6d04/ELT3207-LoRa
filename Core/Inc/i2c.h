#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void i2c1_init(void);
void i2c1_start(void);
void i2c1_stop(void);
void i2c1_write(uint8_t data);
uint8_t I2C1_Read_Ack(void);
uint8_t I2C1_Read_Nack(void);
void i2c1_writebyteto(uint8_t addr, uint8_t data);

#endif
