/**
 * @file OV7670_SCCB_comm.c
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Source file for SCCB communication functions
 * @date 2023-08-01 
 */

#include "OV7670_SCCB_comm.h"
#include "OV7670_debug.h"

#define I2C_TIMEOUT         (25)

/**
 * @brief Driver's I2C communication handle variable
 */
I2C_HandleTypeDef OV7670_hi2c;

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