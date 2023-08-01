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

#include <stdint.h>
#include "stm32f3xx_hal.h"
#include "OV7670_config.h"


/************** Data Structures **************/

/**
 * @brief Simple structure to characterize a pin.
 * @note Created for use in higher level structure OV7670_pins_t.
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

typedef enum {
    OV7670_NO_ERR                       = 0,
    OV7670_GPIO_INVALID_PORT,
    OV7670_GPIO_INVALID_PROPERTIES,
    OV7670_GPIO_CLOCK_DISABLED,
    OV7670_INT_PRIO_GRP_CONFLICT,
} OV7670_error_t;


/***************** Adresses ******************/

#define ADDR_WRITE          (0x42)
#define ADDR_READ           (0x43)


/****************** Macros *******************/

#define OV7670_PIN_DEF(port, num)   {.PORT = port, .NUM = num}
#define OV7670_ERROR_CHECK(fct)                                 \
            do {                                                \
                OV7670_error_t err = fct;                       \
                if (err != HAL_OK) {                            \
                    return(err);                                \
                }                                               \
            } while(0U);

/************ Function prototypes ************/

/**
 * @brief Check if the given GPIO port clock is enabled
 * 
 * @param PORT GPIO port (GPIOx)
 * @return NO_ERR if clock enable, else ERR_GPIO_CLOCK_DISABLED
 */
OV7670_error_t OV7670_check_gpio_clock_en(const GPIO_TypeDef *PORT);



#ifdef __cplusplus
}
#endif

#endif // __OV7670_DRIVER__