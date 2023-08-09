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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "stm32f3xx_hal.h"
#include "OV7670_config.h"


/** @addtogroup OV7670_Driver
  * @{
  */

/** @addtogroup OV7670 types
  * @{
  */

/**
 * @brief Simple structure to characterize a pin.
 * @note Created for use in higher level structure OV7670_pins_t.
 */
typedef struct {
    GPIO_TypeDef *PORT;   // Pin port (GPIOA, GPIOB, etc.)
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

/**
 * @brief Error types
 * 
 * @note * Compatible with HAL_StatusTypeDef status.
 * @note * Use OV7670_print_error() from OV7670_utils.c to print an
 * error message via uart from a given error code.
 */
typedef enum {
    OV7670_NO_ERR                       = 0,
    OV7670_HAL_ERROR,                         // For compatibility with HAL status
    OV7670_HAL_BUSY,                          // For compatibility with HAL status
    OV7670_HAL_TIMEOUT,                       // For compatibility with HAL status
    OV7670_NULL_POINTER,
    OV7670_GPIO_INVALID,
    OV7670_GPIO_CLOCK_DISABLED,
    OV7670_INT_PRIO_GRP_CONFLICT,
    OV7670_UART_ERROR,
    OV7670_PLL_MODIF_FORBIDDEN,
    OV7670_PLL_INVALID_FREQ,
} OV7670_error_t;
/**
  * @}
  */


/* Registers ******************************************************************/
/** @defgroup Registers & commands
  * @{
  */

/**
 * @brief OV7670 Camera module base adresses.
 */
#define ADDR_DEVICE             (0x21)
#define ADDR_WRITE              ((ADDR_DEVICE<<1) | 0x00U)
#define ADDR_READ               ((ADDR_DEVICE<<1) | 0x01U)
/**
 * @brief Register adresses
 */
#define ADDR_GAIN               (0x00U)
#define ADDR_BLUE               (0x01U)
#define ADDR_RED                (0x02U)
#define ADDR_VREF               (0x03U)
#define ADDR_COM1               (0x04U)
#define ADDR_BAVE               (0x05U)
#define ADDR_GbAVE              (0x06U)
#define ADDR_AECHH              (0x07U)
#define ADDR_RAVE               (0x08U)
#define ADDR_COM2               (0x09U)
#define ADDR_PID                (0x0AU)
#define ADDR_VER                (0x0BU)
#define ADDR_COM3               (0x0CU)
#define ADDR_COM4               (0x0DU)
#define ADDR_COM5               (0x0EU)
#define ADDR_COM6               (0x0FU)
#define ADDR_AECH               (0x10U)
#define ADDR_CLKRC              (0x11U)
#define ADDR_COM7               (0x12U)
#define ADDR_COM8               (0x13U)
#define ADDR_COM9               (0x14U)
#define ADDR_COM10              (0x15U)
// 0x16                         Reserved
#define ADDR_HSTART             (0x17U)
#define ADDR_HSTOP              (0x18U)
#define ADDR_VSTRT              (0x19U)
#define ADDR_VSTOP              (0x1AU)
#define ADDR_PSHFT              (0x1BU)
#define ADDR_MIDH               (0x1CU)
#define ADDR_MIDL               (0x1DU)
#define ADDR_MVFP               (0x1EU)
// 0x1F                         Reserved
#define ADDR_ADCCTR0            (0x20U)
// 0x21-0x23                    Reserved
#define ADDR_AEW                (0x24U)
#define ADDR_AEB                (0x25U)
#define ADDR_VPT                (0x26U)
#define ADDR_BBIAS              (0x27U)
#define ADDR_GbBIAS             (0x28U)
// 0x29                         Reserved
#define ADDR_EXHCH              (0x2AU)
#define ADDR_EXHCL              (0x2BU)
#define ADDR_RBIAS              (0x2CU)
#define ADDR_ADVFL              (0x2DU)
#define ADDR_ADVFH              (0x2EU)
#define ADDR_YAVE               (0x2FU)
#define ADDR_HSYST              (0x30U)
#define ADDR_HSYEN              (0x31U)
#define ADDR_HREF               (0x32U)
#define ADDR_CHLF               (0x33U)
#define ADDR_ARBLM              (0x34U)
// 0x35-0x36                    Reserved
#define ADDR_ADC                (0x37U)   // ADC control
#define ADDR_ACOM               (0x38U)
#define ADDR_OFON               (0x39U)
#define ADDR_TSLB               (0x3AU)
#define ADDR_COM11              (0x3BU)
#define ADDR_COM12              (0x3CU)
#define ADDR_COM13              (0x3DU)
#define ADDR_COM14              (0x3EU)
#define ADDR_EDGE               (0x3FU)
#define ADDR_COM15              (0x40U)
#define ADDR_COM16              (0x41U)
#define ADDR_COM17              (0x42U)
#define ADDR_AWBC1              (0x43U)
#define ADDR_AWBC2              (0x44U)
#define ADDR_AWBC3              (0x45U)
#define ADDR_AWBC4              (0x46U)
#define ADDR_AWBC5              (0x47U)
#define ADDR_AWBC6              (0x48U)
// 0x49-0x4A                    Reserved
#define ADDR_REG4B              (0x4BU)
#define ADDR_DNSTH              (0x4CU)
#define ADDR_DM_POS             (0x4DU)
// 0x4E                         Reserved
#define ADDR_MTX1               (0x4FU)
#define ADDR_MTX2               (0x50U)
#define ADDR_MTX3               (0x51U)
#define ADDR_MTX4               (0x52U)
#define ADDR_MTX5               (0x53U)
#define ADDR_MTX6               (0x54U)
#define ADDR_BRIGHT             (0x55U)
#define ADDR_CONTRAST           (0x56U)
#define ADDR_CONTRASCENTER      (0x57U)
#define ADDR_MTXS               (0x58U)
#define ADDR_AWBC7              (0x59U)
#define ADDR_AWBC8              (0x5AU)
#define ADDR_AWBC9              (0x5BU)
#define ADDR_AWBC10             (0x5CU)
#define ADDR_AWBC11             (0x5DU)
#define ADDR_AWBC12             (0x5EU)
#define ADDR_B_LMT              (0x5FU)
#define ADDR_R_LMT              (0x60U)
#define ADDR_G_LMT              (0x61U)
#define ADDR_LCC1               (0x62U)
#define ADDR_LCC2               (0x63U)
#define ADDR_LCC3               (0x64U)
#define ADDR_LCC4               (0x65U)
#define ADDR_LCC5               (0x66U)
#define ADDR_MANU               (0x67U)
#define ADDR_MANV               (0x68U)
#define ADDR_GFIX               (0x69U)
#define ADDR_GGAIN              (0x6AU)
#define ADDR_DBLV               (0x6BU)
#define ADDR_AWBCTR3            (0x6CU)
#define ADDR_AWBCTR2            (0x6DU)
#define ADDR_AWBCTR1            (0x6EU)
#define ADDR_AWBCTR0            (0x6FU)
#define ADDR_SCALING_XSC        (0x70U)
#define ADDR_SCALING_YSC        (0x71U)
#define ADDR_SCALING_DCWCTR     (0x72U)
#define ADDR_SCALING_PCLK_DIV   (0x73U)
#define ADDR_REG74              (0x74U)
#define ADDR_REG75              (0x75U)
#define ADDR_REG76              (0x76U)
#define ADDR_REG77              (0x77U)
// 0x78-0x79                    Reserved
#define ADDR_SLOP               (0x7AU)
#define ADDR_GAM1               (0x7BU)
#define ADDR_GAM2               (0x7CU)
#define ADDR_GAM3               (0x7DU)
#define ADDR_GAM4               (0x7EU)
#define ADDR_GAM5               (0x7FU)
#define ADDR_GAM6               (0x80U)
#define ADDR_GAM7               (0x81U)
#define ADDR_GAM8               (0x82U)
#define ADDR_GAM9               (0x83U)
#define ADDR_GAM10              (0x84U)
#define ADDR_GAM11              (0x85U)
#define ADDR_GAM12              (0x86U)
#define ADDR_GAM13              (0x87U)
#define ADDR_GAM14              (0x88U)
#define ADDR_GAM15              (0x89U)
// 0x8A-0x91                    Reserved
#define ADDR_DM_LNL             (0x92U)
#define ADDR_DM_LNH             (0x93U)
#define ADDR_LCC6               (0x94U)
#define ADDR_LCC7               (0x95U)
// 0x96-0x9C                    Reserved
#define ADDR_BD50ST             (0x9DU)
#define ADDR_BD60ST             (0x9EU)
#define ADDR_HRL                (0x9FU)
#define ADDR_LRL                (0xA0U)
#define ADDR_DSPC3              (0xA1U)
#define ADDR_SCALING_PCLK_DELAY (0xA2U)
// 0xA3                         Reserved
#define ADDR_NT_CTRL            (0xA4U)
#define ADDR_AECGMAX            (0xA5U)
#define ADDR_LPH                (0xA6U)
#define ADDR_UPL                (0xA7U)
#define ADDR_TPL                (0xA8U)
#define ADDR_TPH                (0xA9U)
#define ADDR_NALG               (0xAAU)
// 0xAB                         Reserved
#define ADDR_STROPT             (0xACU)
#define ADDR_STR_R              (0xADU)
#define ADDR_STR_G              (0xAEU)
#define ADDR_STR_B              (0xAFU)
// 0xB0                         Reserved
#define ADDR_ABLC1              (0xB1U)
// 0xB2                         Reserved
#define ADDR_THL_ST             (0xB4U)
// 0xB4                         Reserved
#define ADDR_THL_DLT            (0xB5U)
// 0xB6-BD                      Reserved
#define ADDR_ADCHB              (0xBEU)
#define ADDR_ADCHR              (0xBFU)
#define ADDR_ADCHGb             (0xC0U)
#define ADDR_ADCHGr             (0xC1U)
// 0xC2-0xC8                    Reserved
#define ADDR_SATCTR             (0xC9U)


/**
 * @brief Register content.
*/
#define RESET                   (1U<<7)     // Reset setting
#define STANDBY                 (1U<<4)     // Standby mode setting
#define CCIR656                 (1U<<6)     // CCIR656 format
/**
  * @}
  */

/* Globals ********************************************************************/
/** @defgroup Global variables / functions
  * @{
  */
extern I2C_HandleTypeDef OV7670_hi2c;
extern OV7670_error_t OV7670_err;

extern void assert_failed(uint8_t* file, uint32_t line);
/**
  * @}
  */

/* Utility macros *************************************************************/
/** @defgroup Utility macros
  * @{
  */
#define OV7670_PIN_DEF(port, num)   {.PORT = port, .NUM = num}

#ifdef OV7670_DEBUG 
#define OV7670_CHECK_POINTER(ptr)                                   \
            do {                                                    \
                if (ptr == NULL) {                                  \
                    OV7670_err = OV7670_NULL_POINTER;               \
                    assert_failed((uint8_t *)__FILE__, __LINE__);   \
                }                                                   \
            } while (0U)
#define OV7670_LOG_ERROR(fct)                                       \
            do {                                                    \
                OV7670_err = (OV7670_error_t)fct;                   \
                if (OV7670_err != OV7670_NO_ERR) {                  \
                    assert_failed((uint8_t *)__FILE__, __LINE__);   \
                }                                                   \
            } while (0U)
#else                                     
#define OV7670_POINTER_CHECK(ptr)   do {} while (0)
#define OV7670_LOG_ERROR(fct)       fct
#endif
/**
  * @}
  */


/** @defgroup Function prototypes
  * @{
  */

/* Utility functions **********************************************************/

#ifdef OV7670_DEBUG
/**
 * @brief Convert a status code to a status message and transmit
 * it over to UART.
 * 
 * @param huart Pointer to UART handle
 */
void OV7670_print_error(UART_HandleTypeDef *huart);
#endif

/* Initialization functions ***************************************************/
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

/* Core functions *************************************************************/

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

/**
 * @brief Update a register without affecting the already set register's bits.
 * 
 * @param reg Register to update
 * @param data Value to write
 * @param en 1 to add the bits, 0 to remove them from the register
 */
void OV7670_update_register(uint8_t reg, uint8_t data, uint8_t set);

/* Interrupt Service Routines *************************************************/

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

/**
 * @brief Common command macros
 * 
 * @note I'm creating those macros as I develop and use the driver. Hence, some
 * commands may be missing. Feel free to look at the register map in the OV7670
 * datasheet to find the right register and shift to apply for a given command.
 */

/**
 * @brief Reset the camera module via SCCB
 * @note Another way to reset the module is to use the RST pin.
 */
#define OV7670_RESET_CAMERA()                                                      \
                                    do {                                           \
                                        OV7670_write_register(ADDR_COM7, RESET);   \
                                        HAL_Delay(1);                              \
                                    } while (0U)
#define OV7670_STANDBY_CAMERA()     OV7670_write_register(ADDR_COM2, STANDBY)
#define OV7670_SET_GAIN(byte)       OV7670_write_register(ADDR_GAIN, byte)
#define OV7670_SET_BLUE_GAIN(byte)  OV7670_write_register(ADDR_BLUE, byte)
#define OV7670_SET_RED_GAIN(byte)   OV7670_write_register(ADDR_RED, byte)

#define OV7670_ENABLE_CCIR656()     OV7670_update_register(ADDR_COM1, CCIR656, 1);
#define OV7670_DISABLE_CCIR656()    OV7670_update_register(ADDR_COM1, CCIR656, 0);


/**
  * @}
  */

/**
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif // __OV7670_DRIVER__