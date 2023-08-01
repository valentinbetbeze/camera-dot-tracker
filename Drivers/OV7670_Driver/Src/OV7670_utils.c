#include "OV7670_driver.h"


OV7670_error_t OV7670_check_gpio_clock_en(const GPIO_TypeDef *PORT)
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

// TODO:
void OV7670_get_error_type(const OV7670_error_t err)
{

}