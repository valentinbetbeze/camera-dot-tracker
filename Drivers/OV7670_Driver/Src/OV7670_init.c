/**
 * @file OV7670_init.c
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Source file grouping initialization functions.
 * @date 2023-08-01 
 */

#include "OV7670_driver.h"


#define I2C_TIMING          (0x2000090E)    // Computed by CubeMX
#define I2C_TRIALS          (3)             // number of trials before ready test fails
#define I2C_TIMEOUT         (50)            // ms timeout


/**
 * @brief Initialize GPIOs for the scope of the driver
 * 
 * @param PIN_SCL Serial Clock GPIO used
 * @param PIN_SDA Serial Data GPIO used
 * @return OV7670_status_t error code
 */
static OV7670_status_t OV7670_init_GPIO(const pin_t *pin_scl,
                                        const pin_t *pin_sda)
{
#ifdef OV7670_DEBUG
    // Check arguments
    if (!IS_GPIO_AF_INSTANCE(pin_scl->PORT) || !IS_GPIO_PIN(pin_scl->NUM) ||
        !IS_GPIO_AF_INSTANCE(pin_sda->PORT) || !IS_GPIO_PIN(pin_sda->NUM)) {
            return OV7670_GPIO_INVALID_PROPERTIES;
    }
#endif
    // Enable GPIO port clocks
    OV7670_ERROR_CHECK(OV7670_enable_gpio_clock(pin_scl->PORT));
    OV7670_ERROR_CHECK(OV7670_enable_gpio_clock(pin_sda->PORT));
    // Initialize SCL properties
    const GPIO_InitTypeDef PIN_SCL_INIT = {
        .Pin = pin_scl->NUM,
        .Mode = GPIO_MODE_AF_OD,
        .Pull = GPIO_NOPULL, // SCCB requires floating pin
        .Speed = GPIO_SPEED_FREQ_HIGH,
    };
    OV7670_ERROR_CHECK(OV7670_set_AF(pin_scl->PORT, &PIN_SCL_INIT));
    HAL_GPIO_Init(pin_scl->PORT, &PIN_SCL_INIT);
    // Initialize SDA properties
    const GPIO_InitTypeDef PIN_SDA_INIT = {
        .Pin = pin_sda->NUM,
        .Mode = GPIO_MODE_AF_OD,
        .Pull = GPIO_NOPULL, // SCCB requires floating pin
        .Speed = GPIO_SPEED_FREQ_HIGH,
    };
    OV7670_ERROR_CHECK(OV7670_set_AF(pin_sda->PORT, &PIN_SDA_INIT));
    HAL_GPIO_Init(pin_sda->PORT, &PIN_SDA_INIT);

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


OV7670_status_t OV7670_init_camera(const OV7670_pins_t *pin)
{
#ifdef OV7670_DEBUG
    // Check pointer state for safe dereferencing
    OV7670_POINTER_CHECK(pin);
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
    OV7670_ERROR_CHECK(OV7670_init_GPIO(&pin->PIN_SCL, &pin->PIN_SDA));
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
    OV7670_ERROR_CHECK(HAL_I2C_IsDeviceReady(&OV7670_hi2c, (ADDR_DEVICE<<1U),
                                             I2C_TRIALS, I2C_TIMEOUT));
    // Send reset command
    OV7670_RESET_CAMERA();
    // Wait 1 ms
    HAL_Delay(1);

    return OV7670_NO_ERR;
}


void OV7670_deinit_camera(const OV7670_pins_t *pin)
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
    HAL_GPIO_DeInit(&pin->PIN_SCL.PORT, pin->PIN_SCL.NUM);
    HAL_GPIO_DeInit(&pin->PIN_SDA.PORT, pin->PIN_SDA.NUM);
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
    // TODO: Put the camera in standby
}

