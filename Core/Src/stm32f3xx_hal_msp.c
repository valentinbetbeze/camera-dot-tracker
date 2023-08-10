/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32f3xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    // Init LED2 (user led) GPIO
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef gpio_init = {
        .Pin = LED2_PIN,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_HIGH
    };
    HAL_GPIO_Init(LED2_PORT, &gpio_init);

    /* System interrupt init*/

}

/**
 * @brief Initialize all hardware related to USART2
 * 
 * @param huart Pointer to USART handle
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART2) {
        // Enable USART2 clock
        __HAL_RCC_USART2_CLK_ENABLE();

        // Enable GPIOA clock
        __HAL_RCC_GPIOA_CLK_ENABLE();

        // Initialize GPIOs
        GPIO_InitTypeDef gpio_init = {
            .Pin = USART2_PIN_TX | USART2_PIN_RX,
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_PULLUP,
            .Speed = GPIO_SPEED_FREQ_LOW,
            .Alternate = GPIO_AF7_USART2
        };
        HAL_GPIO_Init(USART2_PORT_RX_TX, &gpio_init);

        // Initialize UART interrupt
        HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
}
