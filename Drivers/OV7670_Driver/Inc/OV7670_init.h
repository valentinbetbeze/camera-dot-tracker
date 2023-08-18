/**
 * @file OV7670_init.h
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Header file for core features: register communication (SCCB)
 * & driver initialization.
 * @date 2023-07-30
 */

#ifndef __OV7670_INIT__
#define __OV7670_INIT__

#ifdef __cplusplus
extern "C" {
#endif

#include "OV7670_config.h"


/* Data Structures ************************************************************/

#define OV7670_PIN_DEF(port, num)   {.PORT = port, .NUM = num}
/**
 * @brief Simple structure to characterize a pin.
 * @note Created for use in higher level structure OV7670_pins_t.
 */
typedef struct {
    GPIO_TypeDef *PORT;         // Pin port (GPIOA, GPIOB, etc.)
    const uint16_t NUM;         // GPIO_pins (see stm32f3xx_hal_gpio.h)
} pin_t;

/**
 * @brief STM32 pins used for the OV7670 camera module.
 */
typedef struct {
    pin_t PIN_SCL;     // Two-Wire Serial Interface Clock
    pin_t PIN_SDA;     // Two-Wire Serial Interface Data I/O
    pin_t PIN_VSYNC;   // Active High: Frame Valid; indicates active frame
    pin_t PIN_HREF;    // Active High: Line/Data Valid; indicates active pixels
    pin_t PIN_PCLK;    // Pixel Clock output from sensor
    pin_t PIN_D7;      // Pixel Data Output 7 (MSB)
    pin_t PIN_D6;      // Pixel Data Output 6
    pin_t PIN_D5;      // Pixel Data Output 5
    pin_t PIN_D4;      // Pixel Data Output 4
    pin_t PIN_D3;      // Pixel Data Output 3
    pin_t PIN_D2;      // Pixel Data Output 2
    pin_t PIN_D1;      // Pixel Data Output 1
    pin_t PIN_D0;      // Pixel Data Output 0
    pin_t PIN_RET;     // Reset
    pin_t PIN_PWDN;    // Power down
} OV7670_pins_t;


/* Function prototypes ********************************************************/

/**
 * @brief Initialize the camera driver.
 * 
 * @param pin Handle to the set of pins used for the camera module
 * 
 * @note The driver requires PLLCKL at 72 MHz or less.
 */
void OV7670_init_camera(OV7670_pins_t *pin);

/**
 * @brief Deinitialize the camera driver, switching off all related
 * peripherals.
 * 
 * @param pin Handle to the set of pins used for the camera module
 */
void OV7670_deinit_camera(OV7670_pins_t *pin);


#ifdef __cplusplus
}
#endif

#endif // __OV7670_INIT__