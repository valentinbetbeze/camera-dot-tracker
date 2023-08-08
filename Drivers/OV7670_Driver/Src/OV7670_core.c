/**
 * @file OV7670_core.c
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Source file for core functions (update/read state)
 * @date 2023-08-01 
 */

#include "OV7670_driver.h"

#define I2C_TIMEOUT         (25)

/**
 * @brief Read the content of a register (1 byte).
 * 
 * @param reg Register to read
 * @param data Buffer to store the value of the register
 * 
 * @note Read operation uses I2C in polling mode.
 */
static void OV7670_read_register(uint8_t reg, uint8_t *data)
{
    OV7670_CHECK_POINTER(data);
    OV7670_LOG_ERROR(HAL_I2C_Master_Transmit(&OV7670_hi2c, ADDR_WRITE,
                                             &reg, 1, I2C_TIMEOUT));
    OV7670_LOG_ERROR(HAL_I2C_Master_Receive(&OV7670_hi2c, ADDR_READ,
                                            data, 1, I2C_TIMEOUT));
}

void OV7670_write_register(uint8_t reg, uint8_t data)
{
    OV7670_LOG_ERROR(HAL_I2C_Mem_Write_IT(&OV7670_hi2c, ADDR_WRITE,
                                          reg, 1, &data, 1));
}

void OV7670_update_register(uint8_t reg, uint8_t data)
{
    // Get current register value
    uint8_t reg_data = 0;
    OV7670_read_register(reg, &reg_data);
    // Update register's content
    reg_data |= data;
    // Send new register value
    OV7670_write_register(reg, reg_data);
}