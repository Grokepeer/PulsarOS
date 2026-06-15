/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPIO0_Pin GPIO_PIN_4
#define GPIO0_GPIO_Port GPIOE
#define GPIO1_Pin GPIO_PIN_5
#define GPIO1_GPIO_Port GPIOE
#define GPIO2_Pin GPIO_PIN_6
#define GPIO2_GPIO_Port GPIOE
#define GPIO3_Pin GPIO_PIN_13
#define GPIO3_GPIO_Port GPIOC
#define GPIO4_Pin GPIO_PIN_14
#define GPIO4_GPIO_Port GPIOC
#define GPIO5_Pin GPIO_PIN_15
#define GPIO5_GPIO_Port GPIOC
#define DIPSIO1_Pin GPIO_PIN_2
#define DIPSIO1_GPIO_Port GPIOC
#define DIPSIO2_Pin GPIO_PIN_3
#define DIPSIO2_GPIO_Port GPIOC
#define SD_WPS_Pin GPIO_PIN_1
#define SD_WPS_GPIO_Port GPIOA
#define SD_CDS_Pin GPIO_PIN_2
#define SD_CDS_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_3
#define LED2_GPIO_Port GPIOA
#define MOD0IO0_Pin GPIO_PIN_4
#define MOD0IO0_GPIO_Port GPIOC
#define MOD0IO1_Pin GPIO_PIN_5
#define MOD0IO1_GPIO_Port GPIOC
#define MOD0IO2_Pin GPIO_PIN_0
#define MOD0IO2_GPIO_Port GPIOB
#define MOD0IO3_Pin GPIO_PIN_1
#define MOD0IO3_GPIO_Port GPIOB
#define MOD0IO4_Pin GPIO_PIN_2
#define MOD0IO4_GPIO_Port GPIOB
#define MOD0IO5_Pin GPIO_PIN_7
#define MOD0IO5_GPIO_Port GPIOE
#define MOD0IO6_Pin GPIO_PIN_8
#define MOD0IO6_GPIO_Port GPIOE
#define MOD1IO0_Pin GPIO_PIN_9
#define MOD1IO0_GPIO_Port GPIOE
#define MOD1IO1_Pin GPIO_PIN_10
#define MOD1IO1_GPIO_Port GPIOE
#define MOD1IO2_Pin GPIO_PIN_11
#define MOD1IO2_GPIO_Port GPIOE
#define MOD1IO3_Pin GPIO_PIN_12
#define MOD1IO3_GPIO_Port GPIOE
#define MOD1IO4_Pin GPIO_PIN_13
#define MOD1IO4_GPIO_Port GPIOE
#define MOD1IO5_Pin GPIO_PIN_14
#define MOD1IO5_GPIO_Port GPIOE
#define MOD1IO6_Pin GPIO_PIN_15
#define MOD1IO6_GPIO_Port GPIOE
#define LED3_Pin GPIO_PIN_12
#define LED3_GPIO_Port GPIOB
#define MOD2IO0_Pin GPIO_PIN_15
#define MOD2IO0_GPIO_Port GPIOB
#define MOD2IO1_Pin GPIO_PIN_8
#define MOD2IO1_GPIO_Port GPIOD
#define MOD2IO2_Pin GPIO_PIN_9
#define MOD2IO2_GPIO_Port GPIOD
#define MOD2IO3_Pin GPIO_PIN_10
#define MOD2IO3_GPIO_Port GPIOD
#define MOD2IO4_Pin GPIO_PIN_11
#define MOD2IO4_GPIO_Port GPIOD
#define MOD2IO5_Pin GPIO_PIN_12
#define MOD2IO5_GPIO_Port GPIOD
#define MOD2IO6_Pin GPIO_PIN_13
#define MOD2IO6_GPIO_Port GPIOD
#define MOD3IO0_Pin GPIO_PIN_14
#define MOD3IO0_GPIO_Port GPIOD
#define MOD3IO1_Pin GPIO_PIN_15
#define MOD3IO1_GPIO_Port GPIOD
#define MOD3IO2_Pin GPIO_PIN_6
#define MOD3IO2_GPIO_Port GPIOC
#define MOD3IO3_Pin GPIO_PIN_7
#define MOD3IO3_GPIO_Port GPIOC
#define MOD3IO4_Pin GPIO_PIN_8
#define MOD3IO4_GPIO_Port GPIOC
#define MOD3IO5_Pin GPIO_PIN_9
#define MOD3IO5_GPIO_Port GPIOC
#define MOD3IO6_Pin GPIO_PIN_8
#define MOD3IO6_GPIO_Port GPIOA
#define GPIO7_Pin GPIO_PIN_11
#define GPIO7_GPIO_Port GPIOA
#define GPIO6_Pin GPIO_PIN_12
#define GPIO6_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_15
#define LED1_GPIO_Port GPIOA
#define BLE_RST_Pin GPIO_PIN_10
#define BLE_RST_GPIO_Port GPIOC
#define BLE_PWRC_Pin GPIO_PIN_11
#define BLE_PWRC_GPIO_Port GPIOC
#define BLE_STAT_Pin GPIO_PIN_6
#define BLE_STAT_GPIO_Port GPIOB
#define BLE_STAT_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
