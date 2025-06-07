#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void I2C1_Init(void);
void I2C1_Start(void);
void I2C1_Stop(void);
void I2C1_Write(uint8_t data);
uint8_t I2C1_Read_Ack(void);
uint8_t I2C1_Read_Nack(void);
void I2C1_WriteByteTo(uint8_t addr, uint8_t data);

#endif
