/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "stm32f3xx_hal.h"


/* Exported types ------------------------------------------------------------*/
extern UART_HandleTypeDef uart2_handle;
extern uint8_t uart2_buf[];
extern uint8_t *uart2_buf_head;


/* Exported constants --------------------------------------------------------*/
#define USART2_PORT_RX_TX       GPIOA
#define USART2_PIN_TX           GPIO_PIN_2      // PA2
#define USART2_PIN_RX           GPIO_PIN_3      // PA3

#define LED2_PORT               GPIOA
#define LED2_PIN                GPIO_PIN_5      // PA5

#define CAMERA_PORT_SCL         GPIOA
#define CAMERA_PIN_SCL          GPIO_PIN_9      // PA9
#define CAMERA_PORT_SDA         GPIOA
#define CAMERA_PIN_SDA          GPIO_PIN_10     // PA10

#define UART_BUF_SIZE           30


/* Exported macro ------------------------------------------------------------*/
#define ERROR_CHECK(fct)                                                    \
                        do {                                                \
                            HAL_StatusTypeDef err = fct;                    \
                            if (err != HAL_OK) {                            \
                                Error_Handler(__func__, err);               \
                            }                                               \
                        } while(0U);
#define ERROR_CHECK_OV7670(fct)                                             \
                        do {                                                \
                            OV7670_error_t err = fct;                       \
                            if (err != OV7670_NO_ERR) {                     \
                                OV7670_print_error(err, &uart2_handle);     \
                            }                                               \
                        } while(0U);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(const char *func_name, HAL_StatusTypeDef err);



#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
