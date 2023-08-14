/**
 * @file main.c
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief 
 * @date 2023-08-01
 * 
 */

#include "main.h"
#include "OV7670_core.h"
#include "OV7670_register_control.h"


/* Global variables ----------------------------------------------------------*/
/**
 * @brief Handle for the UART2 peripheral
 */
UART_HandleTypeDef uart2_handle;
/**
 * @brief Buffer to be used for UART2 communication (log/debug)
 */
uint8_t uart2_buf[UART_BUF_SIZE] = {0};
uint8_t *uart2_buf_head = uart2_buf;    // Pointer to fill the buffer


/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static HAL_StatusTypeDef USART2_Init(void);

/**
 * @brief Redirects printf() to UART2
 */
int __io_putchar(int ch)
{
    ERROR_CHECK(HAL_UART_Transmit(&uart2_handle, (uint8_t *)&ch, 1, 0xFFFF));
    return ch;
}


int main(void)
{
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    ERROR_CHECK(HAL_Init());

    /* Configure the system clock */
    SystemClock_Config();

    // Initialize USART2 for Serial Monitor
    ERROR_CHECK(USART2_Init()); 
    ERROR_CHECK(HAL_UART_RegisterCallback(&uart2_handle, HAL_UART_RX_COMPLETE_CB_ID,
                                          HAL_UART_RxCpltCallback));
    ERROR_CHECK(HAL_UART_RegisterCallback(&uart2_handle, HAL_UART_TX_COMPLETE_CB_ID,
                                          HAL_UART_TxCpltCallback));

    // Initialize OV7670 Camera module
    OV7670_pins_t CAMERA_PIN = {
        .PIN_SCL = OV7670_PIN_DEF(CAMERA_PORT_SCL, CAMERA_PIN_SCL),
        .PIN_SDA = OV7670_PIN_DEF(CAMERA_PORT_SDA, CAMERA_PIN_SDA),
        // etc.
    };
    OV7670_init_camera(&CAMERA_PIN);
    OV7670_write_register(ADDR_GAIN, 0x40);
    uint8_t rdata = 0;
    OV7670_read_register(ADDR_GAIN, &rdata);
    printf("%i\n", rdata);
    

    while (1) {
        
    }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
static void SystemClock_Config(void)
{
    HAL_StatusTypeDef err;
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

    err = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if (err != HAL_OK) {
        Error_Handler(__func__, err);
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    err = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
    if (err != HAL_OK) {
        Error_Handler(__func__, err);
    }
}

/**
 * @brief Initialize USART2
 */
static HAL_StatusTypeDef USART2_Init(void)
{
    uart2_handle.Instance = USART2;
    uart2_handle.Init.BaudRate = 115200;
    uart2_handle.Init.WordLength = UART_WORDLENGTH_8B;
    uart2_handle.Init.StopBits = UART_STOPBITS_1;
    uart2_handle.Init.Parity = UART_PARITY_NONE;
    uart2_handle.Init.Mode = UART_MODE_TX_RX;
    uart2_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uart2_handle.Init.OverSampling = UART_OVERSAMPLING_16;
    uart2_handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    return HAL_UART_Init(&uart2_handle);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(const char *func_name, HAL_StatusTypeDef err)
{
    // Log HAL error message via UART
    switch (err) {
        case HAL_ERROR:
            printf("error: HAL_ERROR returned from %s()", func_name);
            break;
        case HAL_BUSY:
            printf("error: HAL_BUSY returned from %s()", func_name);
            break;
        case HAL_TIMEOUT:
            printf("error: HAL_TIMEOUT returned from %s()", func_name);
            break;
        default:
            printf("error: HAL_OK returned from %s()", func_name);
            break;
    }
    // Disable interrupts
    __disable_irq();
    // Freeze
    while (1);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    printf("%s: line %lu --> assertion failed:\n", file, line);
#ifdef OV7670_DEBUG
    OV7670_print_error(&uart2_handle);
#endif
    while (1); // Freeze
}
#endif /* USE_FULL_ASSERT */
