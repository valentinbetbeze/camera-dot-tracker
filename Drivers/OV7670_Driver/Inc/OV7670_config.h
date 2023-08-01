/**
 * @file OV7670_config.h
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Configuration file for the OV7670 driver.
 * @date 2023-08-01
 */

#ifndef __OV7670_CONFIG__
#define __OV7670_CONFIG__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Uncomment the I2C instance that is to be used for communicating
 * with the 0V7670 registers.
 * @warning Only one I2C instance shall be used at a time.
 * @note Default value is I2C1.
 */
#define OV7670_I2C1         (I2C1)
/* #define OV7670_I2C2         (I2C2) */
/* #define OV7670_I2C3         (I2C3) */


#define I2C_PRIORITY        2       // Interrupt priority for I2C communication





#ifndef OV7670_I2C1 || OV7670_I2C2 || OV7670_I2C3
#error An I2C instance must be chosen
#endif

#ifdef __cplusplus
}
#endif

#endif // __OV7670_CONFIG__