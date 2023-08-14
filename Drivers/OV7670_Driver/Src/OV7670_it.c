/**
 * @file OV7670_it.c
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Source file for interrupt service routines
 * @date 2023-08-01 
 */

#include "OV7670_core.h"

/**
 * @brief This function handles I2C1 event global interrupt /
 * I2C1 wake-up interrupt through EXTI line 23.
*/
#if defined(OV7670_I2C1)
void I2C1_EV_IRQHandler(void) {
#elif defined(OV7670_I2C2)
void I2C2_EV_IRQHandler(void) {
#elif defined(OV7670_I2C3)
void I2C3_EV_IRQHandler(void) {
#endif
    HAL_I2C_EV_IRQHandler(&OV7670_hi2c);
}

/**
 * @brief This function handles I2C1 error interrupt.
*/
#if defined(OV7670_I2C1)
void I2C1_ER_IRQHandler(void) {
#elif defined(OV7670_I2C2)
void I2C2_ER_IRQHandler(void) {
#elif defined(OV7670_I2C3)
void I2C3_ER_IRQHandler(void) {
#endif
    HAL_I2C_ER_IRQHandler(&OV7670_hi2c);
}
