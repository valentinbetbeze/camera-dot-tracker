/**
 * @file OV7670_core.c
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Source file for core functions (update/read state)
 * @date 2023-08-01 
 */

#include "OV7670_driver.h"


void OV7670_read_register(const uint8_t reg, uint8_t *reg_data)
{
    HAL_I2C_Mem_Read_IT(&OV7670_hi2c, ADDR_READ, reg, 1, reg_data, 1);
}


void OV7670_write_register(const uint8_t reg, const uint8_t reg_data)
{
    // Get current register value
    uint8_t cur_reg_data;
    OV7670_read_register(reg, &cur_reg_data);
    // Update register's content
    uint8_t new_reg_data = cur_reg_data | reg_data;
    // Send new register value
    HAL_I2C_Mem_Write_IT(&OV7670_hi2c, ADDR_WRITE, reg, 1, &new_reg_data, 1);
}