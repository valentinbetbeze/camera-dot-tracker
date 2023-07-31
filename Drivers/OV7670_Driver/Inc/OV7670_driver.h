/**
 * @file OV7670_driver.h
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Header file for the custom OV7670 camera sensor driver.
 * @date 2023-07-30
 * 
 * @warning Compatible with STM32F3 family only
 * 
 */

#ifndef __OV7670_DRIVER__
#define __OV7670_DRIVER__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f3xx_hal.h"
#include <stdint.h>


/**
 * @brief Uncomment the I2C instance that is to be used
 * for communicating with the 0V7670 registers.
 * @warning Only one I2C instance shall be used at a time.
 * @note Default value is I2C1.
 */
#define OV7670_I2C1         (I2C1)
/* #define OV7670_I2C2         (I2C2) */
/* #define OV7670_I2C3         (I2C3) */

#ifndef OV7670_I2C1 || OV7670_I2C2 || OV7670_I2C3
#error An I2C instance must be chosen
#endif


#define ADDR_WRITE          (0x42)
#define ADDR_READ           (0x43)


/************ Data Structures ************/

/**
 * @brief Simple structure to characterize a pin.
 */
typedef struct {
    const GPIO_TypeDef *PORT;   // Pin port (GPIOA, GPIOB, etc.)
    const uint16_t NUM;         // Pin number (0..15)
} pin_t;

/**
 * @brief STM32 pins allocated for the OV7670 camera module.
 */
typedef struct {
    const pin_t PIN_SCL;     // Two-Wire Serial Interface Clock
    const pin_t PIN_SDA;     // Two-Wire Serial Interface Data I/O
    const pin_t PIN_VSYNC;   // Active High: Frame Valid; indicates active frame
    const pin_t PIN_HREF;    // Active High: Line/Data Valid; indicates active pixels
    const pin_t PIN_PCLK;    // Pixel Clock output from sensor
    const pin_t PIN_XCLK;    // Master Clock into Sensor
    const pin_t PIN_D7;      // Pixel Data Output 7 (MSB)
    const pin_t PIN_D6;      // Pixel Data Output 6
    const pin_t PIN_D5;      // Pixel Data Output 5
    const pin_t PIN_D4;      // Pixel Data Output 4
    const pin_t PIN_D3;      // Pixel Data Output 3
    const pin_t PIN_D2;      // Pixel Data Output 2
    const pin_t PIN_D1;      // Pixel Data Output 1
    const pin_t PIN_D0;      // Pixel Data Output 0
    const pin_t PIN_RET;     // Reset
    const pin_t PIN_PWDN;    // Power down
} OV7670_pins_t;


/************ Function prototypes ************/

/**
 * @brief Check if the given GPIO port clock is enabled
 * 
 * @param PORT GPIO port (GPIOx)
 * @return 1 if clock is enabled; else 0
 */
uint8_t OV7670_check_gpio_clock(GPIO_TypeDef *PORT);



#ifdef __cplusplus
}
#endif

#endif // __OV7670_DRIVER__