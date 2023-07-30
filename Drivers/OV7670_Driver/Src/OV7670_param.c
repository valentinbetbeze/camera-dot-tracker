#include "OV7670_driver.h"

static I2C_HandleTypeDef OV7670_hi2c = {0};

// Initialize I2C
static void OV7670_init_i2c(const GPIO_TypeDef *PORT,
                            const uint16_t PIN_SCL,
                            const uint16_t PIN_SDA)
{
    // Enable I2C clock
    if (__HAL_RCC_I2C1_IS_CLK_DISABLED()) {
        __HAL_RCC_I2C1_CLK_ENABLE();
    }
    // Configure GPIOs
    const GPIO_InitTypeDef PIN_SCL_INIT = {
        .Pin = PIN_SCL,
        .Mode = ,
        .Pull = ,
        .Speed = ,
        .Alternate = 
    };
    const GPIO_InitTypeDef PIN_SDA_INIT = {
        .Pin = PIN_SDA,
        .Mode = ,
        .Pull = ,
        .Speed = ,
        .Alternate = 
    };
    HAL_GPIO_Init(PORT, &PIN_SCL_INIT);
    HAL_GPIO_Init(PORT, &PIN_SDA_INIT);
    // Configure NVIC

    // Initialize I2C
    const I2C_InitTypeDef I2C_INIT_HANDLER = {
        .Timing = ,
        .OwnAddress1 = ,
        .AddressingMode = ,
        .DualAddressMode = ,
        .OwnAddress2 = ,
        .OwnAddress2Masks = ,
        .GeneralCallMode = ,
        .NoStretchMode = 

    };
    OV7670_hi2c.Instance = 0;
    OV7670_hi2c.Init = I2C_INIT_HANDLER;
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
void OV7670_init_camera(const OV7670_pin_t *PIN)
{
    OV7670_init_i2c(PIN->PORT_I2C, PIN->PIN_SCL, PIN->PIN_SDA);

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