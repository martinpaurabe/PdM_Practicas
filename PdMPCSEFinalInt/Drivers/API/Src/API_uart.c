/*
 * API_uart.c
 *
 *  Created on: Dec 5, 2023
 *      Author: martin
 */

#include "main.h"
#include "API_uart.h"

UART_HandleTypeDef huart2;

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
uint8_t rxBuffer[50];
uint8_t *prxbfr=rxBuffer;
uint8_t rxbfrcant=0;
//enum {PARSER_PRINC, PARSER_LENGTH, PARSER_DATA, PARSER_EOF};
uint8_t rxStt =PARSER_PRINC;
uint8_t NxtRxCant = 1;

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
  HAL_UART_Receive_IT(&huart2,rxBuffer, 1);

  /* USER CODE END USART2_Init 2 */

}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  switch(rxStt)
    {
    case PARSER_PRINC:
      if(rxBuffer[rxbfrcant] == '#')
      {
    	  rxbfrcant++;
    	  rxStt = PARSER_LENGTH;
    	  NxtRxCant = 1;
      }
    break;
    case PARSER_LENGTH:
        if(rxBuffer[rxbfrcant] <20)
        {
          NxtRxCant = rxBuffer[rxbfrcant];
      	  rxbfrcant++;
      	  rxStt = PARSER_DATA;
        }
   break;
    case PARSER_DATA:
    	  rxbfrcant+=NxtRxCant;
    	  rxStt = PARSER_EOF;
    	  NxtRxCant = 1;

    break;
    case PARSER_EOF:
      if(rxBuffer[rxbfrcant] == 255)
      {
    	  HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
    	  sciDataReceived(rxBuffer+2);
      }
      rxStt = PARSER_PRINC;
	  NxtRxCant = 1;
	  rxbfrcant =0;
    break;
    }
    HAL_UART_Receive_IT(huart, rxBuffer+rxbfrcant,NxtRxCant);
   /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback could be implemented in the user file
   */
}

/**********************************************************************
*WriteBytes Function: send the amount of bytes especified by @param n
*********************************************************************/
int32_t WriteBytes(void *Buffer, int32_t n) //Escritura de n en el buffer de entrada
{
	if(HAL_OK == HAL_UART_Transmit(&huart2, (const uint8_t *)Buffer,n,n*1))
		return 1;
	return 0;
}
