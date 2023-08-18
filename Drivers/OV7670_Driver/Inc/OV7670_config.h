/**
 * @file OV7670_config.h
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Configuration file for the OV7670 driver.
 * @date 2023-08-01
 * 
 * @note Update parameters from this configuration header with
 * to fit with the specifications of your project.
 */

#ifndef __OV7670_CONFIG__
#define __OV7670_CONFIG__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "stm32f3xx_hal.h"

/**
 * @brief Comment to disable debug mode.
 * @warning Disabling debug mode will bypass input verifications,
 * error and exception handling from all functions.
 */
#define OV7670_DEBUG        (SPI_1LINE_TX)

/**
 * @brief Error_Handler external reference
 * 
 * @warning Modify the function prototype and arguments according to your
 * custom implementation.
 */
extern void Error_Handler(const char *func_name, HAL_StatusTypeDef err);
#define handle_error        Error_Handler(__func__, err)

/**
 * @brief Uncomment the I2C instance that is to be used for communicating
 * with the 0V7670 registers.
 * @warning Only one I2C instance shall be used at a time.
 * @note Default value is I2C1.
 */
/* #define OV7670_I2C1         (I2C1) */
#define OV7670_I2C2         (I2C2)
/* #define OV7670_I2C3         (I2C3) */

/**
 * @brief Driver's parameters that the user can modify
 */
#define I2C_PRIORITY        2       // Interrupt priority for I2C communication




#ifdef OV7670_DEBUG
#define HAL_CHECK(fct)                                              \
                            do {                                    \
                                HAL_StatusTypeDef err = fct;        \
                                if (err != HAL_OK) {                \
                                    handle_error;                   \
                                }                                   \
                            } while (0U)
#else
#define HAL_CHECK(fct)      fct
#endif


#if (!defined(OV7670_I2C1) && !defined(OV7670_I2C2) && !defined(OV7670_I2C3))
#error An I2C instance must be chosen
#endif

#ifdef __cplusplus
}
#endif

#endif // __OV7670_CONFIG__