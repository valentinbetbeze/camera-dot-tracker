/**
 * @file OV7670_SCCB_comm.h
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Header file for SCCB communication functions
 * @date 2023-08-10
 */

#ifndef __OV7670_SCCB_COMM__
#define __OV7670_SCCB_COMM__

#ifdef __cplusplus
extern "C" {
#endif

#include "OV7670_config.h"


extern I2C_HandleTypeDef OV7670_hi2c;

/**
 * @brief OV7670 Camera module base adresses.
 */
#define ADDR_DEVICE             (0x21)
#define ADDR_WRITE              ((ADDR_DEVICE<<1) | 0x00U)
#define ADDR_READ               ((ADDR_DEVICE<<1) | 0x01U)

/**
 * @brief Read the content of a register (1 byte).
 * 
 * @param reg Register to read
 * @param data Buffer to store the value of the register
 * 
 * @note Read operation uses I2C in polling mode.
 */
void OV7670_read_register(uint8_t reg, uint8_t *data);

/**
 * @brief Write a byte onto a given register.
 * 
 * @param reg Register to update
 * @param data Value to write
 */
void OV7670_write_register(uint8_t reg, uint8_t data);

#ifdef __cplusplus
}
#endif

#endif // __OV7670_SCCB_COMM__