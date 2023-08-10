/**
 * @file OV7670_utils.c
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Source file for utility functions.
 * @date 2023-08-02 
 */

#include "OV7670_debug.h"


#define DEBUG_MSG_SIZE      40

/**
 * @brief Handle for OV7670_error_t error output.
 */
OV7670_error_t OV7670_err;


#ifdef OV7670_DEBUG
void OV7670_print_error(UART_HandleTypeDef *huart)
{
    // Check if UART is ready
    if ((huart == NULL) || (HAL_UART_GetState(huart) != HAL_UART_STATE_READY)) {
        OV7670_LOG_ERROR(OV7670_UART_ERROR);
    }
    // Generate status message
    char message[DEBUG_MSG_SIZE] = {0};
    switch (OV7670_err) {
        case OV7670_NO_ERR:
            strcpy(message, "OV7670_NO_ERR / HAL_OK\n");
            break;
        case OV7670_HAL_ERROR:
            strcpy(message, "HAL_ERROR\n");
            break;
        case OV7670_HAL_BUSY:
            strcpy(message, "HAL_BUSY\n");
            break;
        case OV7670_HAL_TIMEOUT:
            strcpy(message, "HAL_BUSY\n");
            break;
        case OV7670_NULL_POINTER:
            strcpy(message, "OV7670_NULL_POINTER\n");
            break;
        case OV7670_GPIO_INVALID:
            strcpy(message, "OV7670_GPIO_INVALID\n");
            break;
        case OV7670_GPIO_CLOCK_DISABLED:
            strcpy(message, "OV7670_GPIO_CLOCK_DISABLED\n");
            break;
        case OV7670_INT_PRIO_GRP_CONFLICT:
            strcpy(message, "OV7670_INT_PRIO_GRP_CONFLICT\n");
            break;
        case OV7670_UART_ERROR:
            strcpy(message, "OV7670_UART_ERROR\n");
            break;
        default:
            strcpy(message, "unknown_state\n");
            break;
    }
    // Send status
    const uint8_t TIMEOUT = 25; // ms
    HAL_UART_Transmit(huart, (uint8_t *)message, DEBUG_MSG_SIZE, TIMEOUT);
}
#endif