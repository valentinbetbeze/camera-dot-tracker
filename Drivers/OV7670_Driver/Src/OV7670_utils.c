#include "OV7670_driver.h"

uint8_t OV7670_check_gpio_clock(const GPIO_TypeDef *PORT)
{
    switch ((int)PORT) {
        case (int)GPIOA: return __HAL_RCC_GPIOA_IS_CLK_ENABLED();
        case (int)GPIOB: return __HAL_RCC_GPIOB_IS_CLK_ENABLED();
        case (int)GPIOC: return __HAL_RCC_GPIOC_IS_CLK_ENABLED();
        case (int)GPIOD: return __HAL_RCC_GPIOD_IS_CLK_ENABLED();
        case (int)GPIOE: return __HAL_RCC_GPIOE_IS_CLK_ENABLED();
        case (int)GPIOF: return __HAL_RCC_GPIOF_IS_CLK_ENABLED();
        case (int)GPIOG: return __HAL_RCC_GPIOG_IS_CLK_ENABLED();
        case (int)GPIOH: return __HAL_RCC_GPIOH_IS_CLK_ENABLED();
        default: return 1;
    }
    // TODO: if above is functional, remove below
    /*
    if (PORT == GPIOA) return __HAL_RCC_GPIOA_IS_CLK_ENABLED();
    if (PORT == GPIOB) return __HAL_RCC_GPIOB_IS_CLK_ENABLED();
    if (PORT == GPIOC) return __HAL_RCC_GPIOC_IS_CLK_ENABLED();
    if (PORT == GPIOD) return __HAL_RCC_GPIOD_IS_CLK_ENABLED();
    if (PORT == GPIOE) return __HAL_RCC_GPIOE_IS_CLK_ENABLED();
    if (PORT == GPIOF) return __HAL_RCC_GPIOF_IS_CLK_ENABLED();
    if (PORT == GPIOG) return __HAL_RCC_GPIOG_IS_CLK_ENABLED();
    if (PORT == GPIOH) return __HAL_RCC_GPIOH_IS_CLK_ENABLED();
    */
}