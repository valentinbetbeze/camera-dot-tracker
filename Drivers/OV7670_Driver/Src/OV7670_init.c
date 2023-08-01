/**
 * @file OV7670_init.c
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Source file grouping initialization functions.
 * @date 2023-08-01 
 */

#include "OV7670_driver.h"


/**
 * @brief Initialize GPIOs for the scope of the driver
 * 
 * @param PIN_SCL Serial Clock GPIO used
 * @param PIN_SDA Serial Data GPIO used
 * @return OV7670_status_t error code
 */
static OV7670_status_t OV7670_init_GPIO(const pin_t *PIN_SCL,
                                       const pin_t *PIN_SDA)
{
#ifdef OV7670_DEBUG
    // Check arguments
    if (!IS_GPIO_AF_INSTANCE(PIN_SCL->PORT) || !IS_GPIO_PIN(PIN_SCL->NUM) ||
        !IS_GPIO_AF_INSTANCE(PIN_SDA->PORT) || !IS_GPIO_PIN(PIN_SDA->NUM)) {
            return OV7670_GPIO_INVALID_PROPERTIES;
    }
    // Check GPIO port clocks
    if (OV7670_check_gpio_clock_en(PIN_SCL->PORT) != OV7670_NO_ERR ||
        OV7670_check_gpio_clock_en(PIN_SDA->PORT) != OV7670_NO_ERR) {
            return OV7670_GPIO_CLOCK_DISABLED;
    }
#endif
    const GPIO_InitTypeDef PIN_SCL_INIT = {
        .Pin = PIN_SCL->NUM,
        .Mode = GPIO_MODE_AF_OD,
        .Pull = GPIO_NOPULL, // SCCB requires floating pin
        .Speed = GPIO_SPEED_FREQ_HIGH,
        .Alternate = GPIO_AF4_I2C1
    };
    const GPIO_InitTypeDef PIN_SDA_INIT = {
        .Pin = PIN_SDA->NUM,
        .Mode = GPIO_MODE_AF_OD,
        .Pull = GPIO_NOPULL, // SCCB requires floating pin
        .Speed = GPIO_SPEED_FREQ_HIGH,
        .Alternate = GPIO_AF4_I2C1
    };
    HAL_GPIO_Init(PIN_SCL->PORT, &PIN_SCL_INIT);
    HAL_GPIO_Init(PIN_SDA->PORT, &PIN_SDA_INIT);

    return OV7670_NO_ERR;
}


/**
 * @brief Initialize the driver's interrupts.
 * 
 * @return OV7670_status_t error code
 * @note The driver requires the NVIC priority grouping of
 * type 4 (NVIC_PRIORITYGROUP_4). The initialization otherwise fails.
 */
static OV7670_status_t OV7670_init_interrupts(void)
{
#ifdef OV7670_DEBUG
    // Check NVIC priority grouping
    if (HAL_NVIC_GetPriorityGrouping() != NVIC_PRIORITYGROUP_4) {
        return OV7670_INT_PRIO_GRP_CONFLICT;
    }
#endif
    // Enable the I2C interrupt
#if defined(OV7670_I2C1)
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, I2C_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, I2C_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
#elif defined(OV7670_I2C2)
    HAL_NVIC_SetPriority(I2C2_EV_IRQn, I2C_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
    HAL_NVIC_SetPriority(I2C2_ER_IRQn, I2C_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
#elif defined(OV7670_I2C3)
    HAL_NVIC_SetPriority(I2C3_EV_IRQn, I2C_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
    HAL_NVIC_SetPriority(I2C3_ER_IRQn, I2C_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);
#endif
}


OV7670_status_t OV7670_init_camera(const OV7670_pins_t *PIN)
{
#ifdef OV7670_DEBUG
    // Check pointer state for safe dereferencing
    OV7670_POINTER_CHECK(PIN);
#endif
    // Enable I2C clock & instance
#if defined(OV7670_I2C1)
    __HAL_RCC_I2C1_CLK_ENABLE();
    OV7670_hi2c.Instance = I2C1;
#elif defined(OV7670_I2C2)
    __HAL_RCC_I2C2_CLK_ENABLE();
    OV7670_hi2c.Instance = I2C2;
#elif defined(OV7670_I2C3)
    __HAL_RCC_I2C3_CLK_ENABLE();
    OV7670_hi2c.Instance = I2C3;
#endif
    // Configure GPIOs
    OV7670_ERROR_CHECK(OV7670_init_GPIO(&PIN->PIN_SCL, &PIN->PIN_SDA));
    // Configure NVIC
    OV7670_ERROR_CHECK(OV7670_init_interrupts());
    // Initialize I2C
    OV7670_hi2c.Init.Timing = I2C_TIMING;
    OV7670_hi2c.Init.OwnAddress1 = 0; // Not a slave (address not required)
    OV7670_hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    OV7670_hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    OV7670_hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    OV7670_hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    OV7670_ERROR_CHECK(HAL_I2C_Init(&OV7670_hi2c));
    OV7670_ERROR_CHECK(HAL_I2CEx_ConfigAnalogFilter(&OV7670_hi2c,
                                                    I2C_ANALOGFILTER_ENABLE));
    OV7670_ERROR_CHECK(HAL_I2CEx_ConfigDigitalFilter(&OV7670_hi2c, 0));
    // Check if the target device is ready
    OV7670_ERROR_CHECK(HAL_I2C_IsDeviceReady(&OV7670_hi2c, (ADDR_BASE<<1U),
                                             I2C_TRIALS, I2C_TIMEOUT));

    return OV7670_NO_ERR;
}


void OV7670_deinit_camera(const OV7670_pins_t *PIN)
{
    // Disable I2C clock & instance
#if defined(OV7670_I2C1)
    __HAL_RCC_I2C1_CLK_DISABLE();
#elif defined(OV7670_I2C2)
    __HAL_RCC_I2C2_CLK_DISABLE();
#elif defined(OV7670_I2C3)
    __HAL_RCC_I2C3_CLK_DISABLE();
#endif
    // Disable GPIOs
    HAL_GPIO_DeInit(&PIN->PIN_SCL.PORT, PIN->PIN_SCL.NUM);
    HAL_GPIO_DeInit(&PIN->PIN_SDA.PORT, PIN->PIN_SDA.NUM);
    // Disable interrupts
#if defined(OV7670_I2C1)
    HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
#elif defined(OV7670_I2C2)
    HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C2_ER_IRQn);
#elif defined(OV7670_I2C3)
    HAL_NVIC_DisableIRQ(I2C3_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C3_ER_IRQn);
#endif
}

