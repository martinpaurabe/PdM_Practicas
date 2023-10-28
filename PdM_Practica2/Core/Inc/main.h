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
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif


/**********************************************************************************************
 * Practica2.h
 *
 *  Created on: Oct 26, 2023
 *      Author: martin
 **********************************************************************************************/



#ifndef INC_PRACTICA2_H_
#define INC_PRACTICA2_H_

typedef uint32_t tick_t; // Qué biblioteca se debe incluir para que esto compile? stdin.h
typedef bool bool_t;	  // Qué biblioteca se debe incluir para que esto compile? stdbool.h
typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;


/**********************************************************************
 * delayInit Function:
 * Receive a pointer to a delay_t type and initialize it with the
 * time duration specify in the duration parameter in usec.
 * Do not initialize the counter and leave the running state on false.
 *********************************************************************/

void delayInit( delay_t * delay, tick_t duration );

/**********************************************************************
 * delayRead Function:
 * This function is used to start the time counter or to verify if the
 * timer has finished.
 * Receive a pointer to a delay_t type to be analized or that we want to launch counter.
 * Send back true value if the time has been completed or false otherwise
 * (if the time has not been arrived or just start to run)
 *********************************************************************/

bool_t delayRead( delay_t * delay );

/**********************************************************************
 * delayWrite Function:
 * This function is used to set the duration time in usec of a delay structure
 *********************************************************************/
void delayWrite( delay_t * delay, tick_t duration );


#endif /* INC_PRACTICA2_H_ */

#endif /* __MAIN_H */
