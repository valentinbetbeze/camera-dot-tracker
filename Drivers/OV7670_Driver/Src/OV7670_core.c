/**
 * @file OV7670_init.c
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Source file grouping initialization functions.
 * @date 2023-08-01 
 */

#include "OV7670_core.h"
#include "OV7670_register_control.h"
#include "OV7670_debug.h"


#define I2C_TIMING          (0x2000090E)    // Computed by CubeMX
#define I2C_TRIALS          (3)             // Number of trials before ready test fails
#define I2C_TIMEOUT         (25)            // ms timeout

#define PLL_TARGET_FREQ     (24000000)      // MCO frequency in Hz

#define OV7670_CHECK_GPIO(port, num)        (IS_GPIO_AF_INSTANCE(port) &&  \
                                             IS_GPIO_PIN(num))


/* Register control ***********************************************************/

/**
 * @brief Driver's I2C communication handle variable
 */
I2C_HandleTypeDef OV7670_hi2c;


void OV7670_read_register(uint8_t reg, uint8_t *data)
{
    OV7670_CHECK_POINTER(data);
    OV7670_LOG_ERROR(HAL_I2C_Master_Transmit(&OV7670_hi2c, ADDR_WRITE,
                                             &reg, 1, I2C_TIMEOUT));
    OV7670_LOG_ERROR(HAL_I2C_Master_Receive(&OV7670_hi2c, ADDR_READ,
                                            data, 1, I2C_TIMEOUT));
}


void OV7670_write_register(uint8_t reg, uint8_t data)
{
    OV7670_LOG_ERROR(HAL_I2C_Mem_Write(&OV7670_hi2c, ADDR_WRITE,
                                       reg, 1, &data, 1, I2C_TIMEOUT));
}


/* Driver initialization ******************************************************/

/**
 * @brief Enable the given GPIO port clock
 * 
 * @param port GPIO port (GPIOx)
 */
static void OV7670_enable_gpio_clock(const GPIO_TypeDef *port)
{
    if (port == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (port == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (port == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
    else if (port == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
    else if (port == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();
    else if (port == GPIOF) __HAL_RCC_GPIOF_CLK_ENABLE();
    else if (port == GPIOG) __HAL_RCC_GPIOG_CLK_ENABLE();
    else if (port == GPIOH) __HAL_RCC_GPIOH_CLK_ENABLE();
#ifdef OV7670_DEBUG
    else OV7670_LOG_ERROR(OV7670_GPIO_INVALID);
#endif
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
    OV7670_CHECK_POINTER(port);
    OV7670_CHECK_POINTER(gpio_init);

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
#ifdef OV7670_DEBUG
    OV7670_CHECK_POINTER(pin);
    // Check arguments
    if (!OV7670_CHECK_GPIO(pin->PIN_SCL.PORT, pin->PIN_SCL.NUM) ||
        !OV7670_CHECK_GPIO(pin->PIN_SDA.PORT, pin->PIN_SDA.NUM)) {
            OV7670_LOG_ERROR(OV7670_GPIO_INVALID);
    }
#endif
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
 */
static void OV7670_config_MCO(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

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

        OV7670_LOG_ERROR(HAL_RCC_OscConfig(&RCC_OscInitStruct));
    }
    HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_1);
}

/**
 * @brief Initialize the driver's interrupts.
 * 
 * @note The driver requires the NVIC priority grouping of
 * type 4 (NVIC_PRIORITYGROUP_4). The initialization otherwise fails.
 */
static void OV7670_init_interrupts(void)
{
#ifdef OV7670_DEBUG
    // Check NVIC priority grouping
    if (HAL_NVIC_GetPriorityGrouping() != NVIC_PRIORITYGROUP_4) {
        OV7670_LOG_ERROR(OV7670_INT_PRIO_GRP_CONFLICT);
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


void OV7670_init_camera(OV7670_pins_t *pin)
{
    // Check pointer state for safe dereferencing
    OV7670_CHECK_POINTER(pin);
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
    // Configure NVIC (unused in this version)
    /* OV7670_init_interrupts(); */

    // Initialize I2C
    OV7670_hi2c.Init.Timing = I2C_TIMING;
    OV7670_hi2c.Init.OwnAddress1 = 0; // Not a slave (address not required)
    OV7670_hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    OV7670_hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    OV7670_hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    OV7670_hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    OV7670_LOG_ERROR(HAL_I2C_Init(&OV7670_hi2c));
    OV7670_LOG_ERROR(HAL_I2CEx_ConfigAnalogFilter(&OV7670_hi2c,
                                                  I2C_ANALOGFILTER_ENABLE));
    OV7670_LOG_ERROR(HAL_I2CEx_ConfigDigitalFilter(&OV7670_hi2c, 0));
    
    // Check if the target device is ready
    OV7670_LOG_ERROR(HAL_I2C_IsDeviceReady(&OV7670_hi2c, (ADDR_DEVICE<<1),
                                           I2C_TRIALS, I2C_TIMEOUT));
    // Send reset command
    OV7670_reset_camera();

    // TODO: any other commands (format, etc)
}


void OV7670_deinit_camera(OV7670_pins_t *pin)
{
    OV7670_CHECK_POINTER(pin);
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

