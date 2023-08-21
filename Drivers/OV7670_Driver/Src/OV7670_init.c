/**
 * @file OV7670_init.c
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Source file grouping initialization functions.
 * @date 2023-08-01 
 */

#include "OV7670_init.h"
#include "OV7670_regctrl.h"


#define ADDR_DEVICE         (0x21)

#define I2C_TIMING          (0x2000090E)    // Computed by CubeMX
#define I2C_TIMEOUT         50
#define I2C_TRIALS          3               // Number of trials before ready test fails


extern I2C_HandleTypeDef OV7670_hi2c;

/**
 * @brief Enable the given GPIO port clock
 * 
 * @param port GPIO port (GPIOx)
 */
static void OV7670_enable_gpio_clock(const GPIO_TypeDef *port)
{
    assert_param(IS_GPIO_ALL_INSTANCE(port));

    if (port == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (port == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (port == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
    else if (port == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
    else if (port == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();
    else if (port == GPIOF) __HAL_RCC_GPIOF_CLK_ENABLE();
    else if (port == GPIOG) __HAL_RCC_GPIOG_CLK_ENABLE();
    else if (port == GPIOH) __HAL_RCC_GPIOH_CLK_ENABLE();
}

/**
 * @brief Assign the I2Cx alternate function corresponding to the
 * given GPIO port.
 * 
 * @param port GPIO port
 * @param gpio_init GPIO initialization handle
 */
static void OV7670_gpio_set_AF(const GPIO_TypeDef *port,
                               GPIO_InitTypeDef *gpio_init)
{
    assert_param(port != NULL);
    assert_param(gpio_init != NULL);

    assert_param(IS_GPIO_AF_INSTANCE(port));

#if defined(OV7670_I2C1)
    gpio_init->Alternate = GPIO_AF4_I2C1;
#elif defined(OV7670_I2C2)
    gpio_init->Alternate = GPIO_AF4_I2C2;
#elif defined(OV7670_I2C3)
    if (port == GPIOA) gpio_init->Alternate = GPIO_AF3_I2C3;
    else if (port == GPIOB) gpio_init->Alternate = GPIO_AF8_I2C3;
    else if (port == GPIOC) gpio_init->Alternate = GPIO_AF3_I2C3;
#endif
}

/**
 * @brief Initialize GPIOs for the scope of the driver
 * 
 * @param pin Handle to the set of pins used for the camera module
 */
static void OV7670_init_GPIO(OV7670_pins_t *pin)
{
    assert_param(pin != NULL);

    // Check GPIO validity
    assert_param(IS_GPIO_AF_INSTANCE(pin->PIN_SCL.PORT));
    assert_param(IS_GPIO_AF_INSTANCE(pin->PIN_SDA.PORT));
    assert_param(IS_GPIO_PIN(pin->PIN_SCL.NUM));
    assert_param(IS_GPIO_PIN(pin->PIN_SDA.NUM));

    // Enable GPIO port clocks
    OV7670_enable_gpio_clock(pin->PIN_SCL.PORT);
    OV7670_enable_gpio_clock(pin->PIN_SDA.PORT);
    
    // Initialize SCL properties
    GPIO_InitTypeDef scl_init = {
        .Pin = pin->PIN_SCL.NUM,
        .Mode = GPIO_MODE_AF_OD,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_LOW,
    };
    OV7670_gpio_set_AF(pin->PIN_SCL.PORT, &scl_init);
    HAL_GPIO_Init(pin->PIN_SCL.PORT, &scl_init);

    // Initialize SDA properties
    GPIO_InitTypeDef sda_init = {
        .Pin = pin->PIN_SDA.NUM,
        .Mode = GPIO_MODE_AF_OD,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW,
    };
    OV7670_gpio_set_AF(pin->PIN_SDA.PORT, &sda_init);
    HAL_GPIO_Init(pin->PIN_SDA.PORT, &sda_init);
}

/**
 * @brief Configure Master Clock Output.
 * 
 * @note GPIO PA8 on STM32F303xE
 */
static void OV7670_config_MCO(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

    // If PLL is ON, keep the current PLLCLK config
    if (RCC_OscInitStruct.PLL.PLLState != RCC_PLL_ON) {

        __HAL_RCC_PLL_DISABLE();

        RCC_OscInitStruct.HSIState = RCC_HSI_ON;
        RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
        // Min: 10 MHz; Nominal: 24 MHz; Max: 48 MHz
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
        RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
        RCC_OscInitStruct.PLL.PLLMUL = RCC_CFGR_PLLMUL3;

        __HAL_RCC_PLL_ENABLE();

        HAL_CHECK(HAL_RCC_OscConfig(&RCC_OscInitStruct));
    }
    HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_4);
}


void OV7670_init_camera(OV7670_pins_t *pin)
{
    assert_param(pin != NULL);

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
    OV7670_init_GPIO(pin);

    // Initialize Master Clock Output (MCO) for XCLK.
    OV7670_config_MCO();

    // Initialize I2C
    OV7670_hi2c.Init.Timing = I2C_TIMING;
    OV7670_hi2c.Init.OwnAddress1 = 0; // Not a slave (address not required)
    OV7670_hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    OV7670_hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    OV7670_hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    OV7670_hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_CHECK(HAL_I2C_Init(&OV7670_hi2c));
    HAL_CHECK(HAL_I2CEx_ConfigAnalogFilter(&OV7670_hi2c,
                                           I2C_ANALOGFILTER_ENABLE));
    HAL_CHECK(HAL_I2CEx_ConfigDigitalFilter(&OV7670_hi2c, 0));

    // Check if the target device is ready
    HAL_CHECK(HAL_I2C_IsDeviceReady(&OV7670_hi2c, (ADDR_DEVICE<<1), I2C_TRIALS,
                                    I2C_TIMEOUT));

    // Send reset command
    OV7670_reset_camera();
}


void OV7670_deinit_camera(OV7670_pins_t *pin)
{
    assert_param(pin != NULL);
    /**
     * If the Power Down pin is set, use it: more robust and provides lower
     * standby current. Else use the COM2 register via the SCCB interface.
     */
    if (IS_GPIO_ALL_INSTANCE(pin->PIN_PWDN.PORT) &&
        IS_GPIO_PIN(pin->PIN_PWDN.NUM)) {
        HAL_GPIO_WritePin(pin->PIN_PWDN.PORT, pin->PIN_PWDN.NUM, GPIO_PIN_SET);
    }
    else {
        OV7670_sleep_camera();
    }
    
    // Disable I2C clock & instance
#if defined(OV7670_I2C1)
    __HAL_RCC_I2C1_CLK_DISABLE();
#elif defined(OV7670_I2C2)
    __HAL_RCC_I2C2_CLK_DISABLE();
#elif defined(OV7670_I2C3)
    __HAL_RCC_I2C3_CLK_DISABLE();
#endif

    // Disable GPIOs
    HAL_GPIO_DeInit(pin->PIN_SCL.PORT, pin->PIN_SCL.NUM);
    HAL_GPIO_DeInit(pin->PIN_SDA.PORT, pin->PIN_SDA.NUM);
}

