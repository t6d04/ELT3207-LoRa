#ifndef __I2C_H
#define __I2C_H

#include <stdint.h>

void I2C1_Init(void);
void I2C1_WriteByte(uint8_t addr_7bit, uint8_t data);

#endif
