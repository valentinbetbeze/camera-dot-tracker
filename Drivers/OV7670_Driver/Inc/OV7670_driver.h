/**
 * @file OV7670_driver.h
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Header file for the custom OV7670 camera sensor driver.
 * @date 2023-07-30
 * 
 * @warning Compatible for STM32F3 family only
 * 
 */

#ifndef __OV7670_DRIVER__
#define __OV7670_DRIVER__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f3xx_hal.h"
#include <stdint.h>

#define ADDR_WRITE       (0x42)
#define ADDR_READ        (0x43)

typedef struct {
    const uint16_t PIN_VCC;     // 3.3V Power supply 
    const uint16_t PIN_GND;     // Power ground
    struct {
        const GPIO_TypeDef *PORT_I2C;   // Port corresponding to I2C pins
        const uint16_t PIN_SCL;     // Two-Wire Serial Interface Clock
        const uint16_t PIN_SDA;     // Two-Wire Serial Interface Data I/O
    };
    const uint16_t PIN_VSYNC;   // Active High: Frame Valid; indicates active frame
    const uint16_t PIN_HREF;    // Active High: Line/Data Valid; indicates active pixels
    const uint16_t PIN_PCLK;    // Pixel Clock output from sensor
    const uint16_t PIN_XCLK;    // Master Clock into Sensor
    const uint16_t PIN_D7;      // Pixel Data Output 7 (MSB)
    const uint16_t PIN_D6;      // Pixel Data Output 6
    const uint16_t PIN_D5;      // Pixel Data Output 5
    const uint16_t PIN_D4;      // Pixel Data Output 4
    const uint16_t PIN_D3;      // Pixel Data Output 3
    const uint16_t PIN_D2;      // Pixel Data Output 2
    const uint16_t PIN_D1;      // Pixel Data Output 1
    const uint16_t PIN_D0;      // Pixel Data Output 0
    const uint16_t PIN_RET;     // Reset
    const uint16_t PIN_PWDN;    // Power down
} OV7670_pin_t;


#ifdef __cplusplus
}
#endif

#endif // __OV7670_DRIVER__