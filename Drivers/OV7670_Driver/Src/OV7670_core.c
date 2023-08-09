/**
 * @file OV7670_core.c
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Source file for core functions (update/read state)
 * @date 2023-08-01 
 */

#include "OV7670_driver.h"

#define I2C_TIMEOUT         (25)


void OV7670_read_register(uint8_t reg, uint8_t *data)
{
    OV7670_CHECK_POINTER(data);
    OV7670_LOG_ERROR(HAL_I2C_Master_Transmit(&OV7670_hi2c, ADDR_WRITE,
                                             &reg, 1, I2C_TIMEOUT));
    OV7670_LOG_ERROR(HAL_I2C_Master_Receive(&OV7670_hi2c, ADDR_READ,
                                            data, 1, I2C_TIMEOUT));
}

void OV7670_write_register(uint8_t reg, uint8_t data)
{
    OV7670_LOG_ERROR(HAL_I2C_Mem_Write(&OV7670_hi2c, ADDR_WRITE,
                                       reg, 1, &data, 1, I2C_TIMEOUT));
}

void OV7670_update_register(uint8_t reg, uint8_t data, uint8_t set)
{
    // Get current register value
    uint8_t regdata = 0;
    OV7670_read_register(reg, &regdata);
    // Update register's content
    regdata = (set) ? regdata | data : regdata & ~data;
    // Send new register value
    OV7670_write_register(reg, regdata);
}