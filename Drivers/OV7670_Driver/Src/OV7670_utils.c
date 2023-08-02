#include "OV7670_driver.h"


OV7670_status_t OV7670_enable_gpio_clock(const GPIO_TypeDef *port)
{
    switch ((int)port) {
        case (int)GPIOA: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
        case (int)GPIOB: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
        case (int)GPIOC: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
        case (int)GPIOD: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
        case (int)GPIOE: __HAL_RCC_GPIOE_CLK_ENABLE(); break;
        case (int)GPIOF: __HAL_RCC_GPIOF_CLK_ENABLE(); break;
        case (int)GPIOG: __HAL_RCC_GPIOG_CLK_ENABLE(); break;
        default:
            return OV7670_GPIO_INVALID_PORT;
        
        return OV7670_NO_ERR;
    }
}

OV7670_status_t OV7670_set_AF(const GPIO_TypeDef *port,
                              GPIO_InitTypeDef *gpio_init)
{
#ifdef OV7670_DEBUG
    OV7670_POINTER_CHECK(port);
    OV7670_POINTER_CHECK(gpio_init);
#endif
#if defined(OV7670_I2C1)
    gpio_init->Alternate = GPIO_AF4_I2C1;
#elif defined(OV7670_I2C2)
    gpio_init->Alternate = GPIO_AF4_I2C2;
#elif defined(OV7670_I2C3)
    switch ((int)port) {
        case (int)GPIOA: gpio_init->Alternate = GPIO_AF3_I2C3; break;
        case (int)GPIOB: gpio_init->Alternate = GPIO_AF8_I2C3; break;
        case (int)GPIOC: gpio_init->Alternate = GPIO_AF3_I2C3; break;
        default:
            return OV7670_GPIO_INVALID_PORT;
    }
#endif
    return OV7670_NO_ERR;
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