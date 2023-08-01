#include "OV7670_driver.h"


OV7670_status_t OV7670_check_gpio_clock_en(const GPIO_TypeDef *PORT)
{
    switch ((int)PORT) {
        case (int)GPIOA:
            return (__HAL_RCC_GPIOA_IS_CLK_ENABLED() == SET) ? \
                    OV7670_NO_ERR : OV7670_GPIO_CLOCK_DISABLED;
        case (int)GPIOB:
            return (__HAL_RCC_GPIOB_IS_CLK_ENABLED() == SET) ? \
                    OV7670_NO_ERR : OV7670_GPIO_CLOCK_DISABLED;
        case (int)GPIOC:
            return (__HAL_RCC_GPIOC_IS_CLK_ENABLED() == SET) ? \
                    OV7670_NO_ERR : OV7670_GPIO_CLOCK_DISABLED;
        case (int)GPIOD:
            return (__HAL_RCC_GPIOD_IS_CLK_ENABLED() == SET) ? \
                    OV7670_NO_ERR : OV7670_GPIO_CLOCK_DISABLED;
        case (int)GPIOE:
            return (__HAL_RCC_GPIOE_IS_CLK_ENABLED() == SET) ? \
                    OV7670_NO_ERR : OV7670_GPIO_CLOCK_DISABLED;
        case (int)GPIOF:
            return (__HAL_RCC_GPIOF_IS_CLK_ENABLED() == SET) ? \
                    OV7670_NO_ERR : OV7670_GPIO_CLOCK_DISABLED;
        case (int)GPIOG:
            return (__HAL_RCC_GPIOG_IS_CLK_ENABLED() == SET) ? \
                    OV7670_NO_ERR : OV7670_GPIO_CLOCK_DISABLED;
        case (int)GPIOH:
            return (__HAL_RCC_GPIOH_IS_CLK_ENABLED() == SET) ? \
                    OV7670_NO_ERR : OV7670_GPIO_CLOCK_DISABLED;
        default:
            return OV7670_GPIO_INVALID_PORT;
    }
}


#ifdef OV7670_DEBUG
OV7670_status_t OV7670_get_status_type(const OV7670_status_t st,
                                       const UART_HandleTypeDef *huart)
{
    OV7670_POINTER_CHECK(huart);
    // Check if UART is ready
    if (HAL_UART_GetState(huart) != HAL_UART_STATE_READY) {
        return OV7670_UART_ERROR;
    }
    // Generate status
    const uint16_t SIZE = 50;
    uint8_t *message = calloc(SIZE, sizeof(uint8_t));

    switch (st) {
        case OV7670_NO_ERR:
            strcpy(message, "OV7670_NO_ERR / HAL_OK\n");
            break;
        case HAL_ERROR:
            strcpy(message, "HAL_ERROR\n");
            break;
        case HAL_BUSY:
            strcpy(message, "HAL_BUSY\n");
            break;
        case HAL_TIMEOUT:
            strcpy(message, "HAL_TIMEOUT\n");
            break;
        case OV7670_NULL_POINTER:
            strcpy(message, "OV7670_NULL_POINTER\n");
            break;
        case OV7670_GPIO_INVALID_PORT:
            strcpy(message, "OV7670_GPIO_INVALID_PORT\n");
            break;
        case OV7670_GPIO_INVALID_PROPERTIES: 
            strcpy(message, "OV7670_GPIO_INVALID_PROPERTIES\n");
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
    OV7670_ERROR_CHECK(HAL_UART_Transmit(huart, message, SIZE, TIMEOUT));

    // Free allocated memory
    free(message);

    return OV7670_NO_ERR;
}
#endif