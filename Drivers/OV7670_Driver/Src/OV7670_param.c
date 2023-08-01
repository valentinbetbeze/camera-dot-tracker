/**
 * @file OV7670_it.c
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Source file grouping all functions used for initializing
 * the driver and updating its parameters.
 * @date 2023-08-01 
 */

#include "OV7670_driver.h"

static I2C_HandleTypeDef OV7670_hi2c = {0};

// Initialize I2C
static OV7670_error_t OV7670_init_i2c(const pin_t *PIN_SCL,
                                      const pin_t *PIN_SDA)
{
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
    // Enable I2C clock
#ifdef OV7670_I2C1
    __HAL_RCC_I2C1_CLK_ENABLE();
#elif defined(OV7670_I2C2)
    __HAL_RCC_I2C2_CLK_ENABLE();
#elif defined(OV7670_I2C3)
    __HAL_RCC_I2C3_CLK_ENABLE();
#endif
    // Configure GPIOs
    const GPIO_InitTypeDef PIN_SCL_INIT = {
        .Pin = PIN_SCL->NUM,
        .Mode = GPIO_MODE_AF_OD,
        .Pull = GPIO_NOPULL, // SCCB requires floating pin
        .Speed = GPIO_SPEED_FREQ_LOW,
    };
    const GPIO_InitTypeDef PIN_SDA_INIT = {
        .Pin = PIN_SDA->NUM,
        .Mode = GPIO_MODE_AF_OD,
        .Pull = GPIO_NOPULL, // SCCB requires floating pin
        .Speed = GPIO_SPEED_FREQ_LOW,
    };
    HAL_GPIO_Init(PIN_SCL->PORT, &PIN_SCL_INIT);
    HAL_GPIO_Init(PIN_SDA->PORT, &PIN_SDA_INIT);
    // Configure NVIC
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
#if defined(OV7670_I2C1)
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, , );
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
#elif defined(OV7670_I2C2)
    HAL_NVIC_SetPriority(I2C2_EV_IRQn, , );
    HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
#elif defined(OV7670_I2C3)
    HAL_NVIC_SetPriority(I2C3_EV_IRQn, , );
    HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
#endif
    // TODO: what about that --> I2C1_EV_IRQHandler / I2C1_ER_IRQHandler
    // Initialize I2C
    OV7670_hi2c.Instance = 0;
    OV7670_hi2c.Init.Timing = ;
    OV7670_hi2c.Init.OwnAddress1 = ;
    OV7670_hi2c.Init.AddressingMode = ;
    OV7670_hi2c.Init.DualAddressMode = ;
    OV7670_hi2c.Init.OwnAddress2 = ;
    OV7670_hi2c.Init.OwnAddress2Masks = ;
    OV7670_hi2c.Init.GeneralCallMode = ;
    OV7670_hi2c.Init.NoStretchMode = ;
    HAL_I2C_Init(&OV7670_hi2c);
    // Check if the target device is ready
    // HAL_I2C_IsDeviceReady(&OV7670_hi2c, , , );

}

// Send command
static void OV7670_transmit_i2c(void)
{

}

// Receive response
static void OV7670_receive_i2c(void)
{

}

// Initialize driver
void OV7670_init_camera(const OV7670_pins_t *PIN)
{
    OV7670_init_i2c(&(PIN->PIN_SCL), &(PIN->PIN_SDA));

}

// Reset camera
void OV7670_reset_camera(void)
{

}

// Put camera to sleep
void OV7670_sleep_camera(void)
{

}

// Wake camera from sleep
void OV7670_wake_camera(void)
{

}

// Read camera parameter
void OV7670_read_parameter(void)
{

}

// Update camera parameter
void OV7670_update_parameter(void)
{

}