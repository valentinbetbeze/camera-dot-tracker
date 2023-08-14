/**
 * @file OV7670_debug.h
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Header file for debug features related to this driver.
 * @date 2023-07-30
 */

#ifndef __OV7670_DEBUG__
#define __OV7670_DEBUG__

#ifdef __cplusplus
extern "C" {
#endif

#include "OV7670_config.h"


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


/* Globals ********************************************************************/

extern OV7670_error_t OV7670_err;
extern void assert_failed(uint8_t* file, uint32_t line);


/* Macros *********************************************************************/

#define OV7670_PIN_DEF(port, num)   {.PORT = port, .NUM = num}
#ifdef OV7670_DEBUG 
#define OV7670_CHECK_POINTER(ptr)                                                           \
                                    do {                                                    \
                                        if (ptr == NULL) {                                  \
                                            OV7670_err = OV7670_NULL_POINTER;               \
                                            assert_failed((uint8_t *)__FILE__, __LINE__);   \
                                        }                                                   \
                                    } while (0U)
#define OV7670_LOG_ERROR(fct)                                                               \
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


/* Function prototypes ********************************************************/

#if (defined(OV7670_DEBUG) && defined(HAL_UART_MODULE_ENABLED))
/**
 * @brief Convert a status code to a status message and transmit
 * it over to UART.
 * 
 * @param huart Pointer to UART handle
 */
void OV7670_print_error(UART_HandleTypeDef *huart);
#endif



#ifdef __cplusplus
}
#endif

#endif // __OV7670_DEBUG__