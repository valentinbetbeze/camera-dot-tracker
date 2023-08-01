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
#include <stdlib.h>
#include <string.h>

#include "stm32f3xx_hal.h"
#include "OV7670_config.h"


/************* Global variables **************/

/**
 * @brief Driver's I2C communication handle variable
 */
I2C_HandleTypeDef OV7670_hi2c = {0};


/************** Data Structures **************/

/**
 * @brief Simple structure to characterize a pin.
 * @note Created for use in higher level structure OV7670_pins_t.
 */
typedef struct {
    const GPIO_TypeDef *PORT;   // Pin port (GPIOA, GPIOB, etc.)
    const uint16_t NUM;         // GPIO_pins (see stm32f3xx_hal_gpio.h)
} pin_t;

/**
 * @brief STM32 pins used for the OV7670 camera module.
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

/**
 * @brief Types of status/erros that can be encountered.
 * @note * Use OV7670_read_status() from OV7670_utils.c to retrieve a
 * status from its code.
 * @note * Compatible with HAL_StatusTypeDef status.
 */
typedef enum {
    OV7670_NO_ERR                       = 0,
    HAL_ERROR,
    HAL_BUSY,
    HAL_TIMEOUT,
    OV7670_NULL_POINTER,
    OV7670_GPIO_INVALID_PORT,
    OV7670_GPIO_INVALID_PROPERTIES,
    OV7670_GPIO_CLOCK_DISABLED,
    OV7670_INT_PRIO_GRP_CONFLICT,
    OV7670_UART_ERROR,
} OV7670_status_t;


/***************** Adresses ******************/

#define ADDR_BASE           (0x21)
#define ADDR_WRITE          (0x42)
#define ADDR_READ           (0x43)

#define I2C_TIMING          (0x2000090E)    // Computed by CubeMX
#define I2C_TRIALS          (3)             // number of trials before ready test fails
#define I2C_TIMEOUT         (50)            // ms timeout


/****************** Macros *******************/

#define OV7670_PIN_DEF(port, num)   {.PORT = port, .NUM = num}
#define OV7670_ERROR_CHECK(fct)                                 \
            do {                                                \
                OV7670_status_t st = fct;                       \
                if (st != OV7670_NO_ERR) {                      \
                    return st;                                  \
                }                                               \
            } while (0U);
#define OV7670_POINTER_CHECK(ptr)                               \
            do {                                                \
                if (ptr == NULL) {                              \
                    return OV7670_NULL_POINTER;                 \
                }                                               \
            } while (0U);


/************ Function prototypes ************/

/**
 * @brief Check if the given GPIO port clock is enabled
 * 
 * @param PORT GPIO port (GPIOx)
 * @return Status code
 */
OV7670_status_t OV7670_check_gpio_clock_en(const GPIO_TypeDef *PORT);

#ifdef OV7670_DEBUG
/**
 * @brief Convert a status code to a status message and transmit
 * it over to UART.
 * 
 * @param st Status code to transmit
 * @param huart Pointer to UART handle
 * @return Status code
 */
OV7670_status_t OV7670_get_status_type(const OV7670_status_t st,
                                       const UART_HandleTypeDef *huart);
#endif

/**
 * @brief Initialize the camera driver.
 * 
 * @param PIN Handle to the set of pins used for the camera module
 * @return Status code
 */
OV7670_status_t OV7670_init_camera(const OV7670_pins_t *PIN);

/**
 * @brief Deinitialize the camera driver, switching off all related
 * peripherals.
 * 
 * @param PIN Handle to the set of pins used for the camera module
 */
void OV7670_deinit_camera(const OV7670_pins_t *PIN);

/**
 * @brief I2Cx ISRs
 */
#if defined(OV7670_I2C1)
void I2C1_EV_IRQHandler(void);
void I2C1_ER_IRQHandler(void);
#elif defined(OV7670_I2C2)
void I2C2_EV_IRQHandler(void);
void I2C2_ER_IRQHandler(void);
#elif defined(OV7670_I2C3)
void I2C3_EV_IRQHandler(void);
void I2C3_ER_IRQHandler(void);
#endif


#ifdef __cplusplus
}
#endif

#endif // __OV7670_DRIVER__