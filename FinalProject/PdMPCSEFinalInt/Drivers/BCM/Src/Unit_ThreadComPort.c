/*
 * Unit_ThreadComPort.c
 *
 *  Created on: Dec 5, 2023
 *      Author: martin
 */
#include "Unit_ThreadComPort.h"

//My Libraries
//#include "API_uart.h"
#include "main.h" //Just to toggle the LED on debbugging state
#include "stm32f4xx_hal.h"

// C library headers
#include <stdio.h>
#include <string.h>
#include <unistd.h>



enum {PARSER_PRINC, PARSER_LENGTH, PARSER_DATA, PARSER_EOF};


UART_HandleTypeDef huart2;

static uint8_t rxBuffer[DIM_ADQ];
static uint8_t rxbfrcant=0;
static uint8_t rxStt =PARSER_PRINC;
static uint8_t NxtRxCant = 1;


static uint8_t txBuf[DIM_ADQ];
static volatile uint8_t txBufIdx;
static uint8_t txCantBytes; //Cantidad de datos a transmitir en buffer

static void MX_USART2_UART_Init(void);
static int32_t WriteBytes(void *Buffer, int32_t n);



/**********************************************************************
* BytesDisponibles Function:
* Return the amount of
*********************************************************************/
void ThreadComPort_Init(void)
{
	MX_USART2_UART_Init();
}

/**********************************************************************
* BytesDisponibles Function:
* Return the amount of
*********************************************************************/

bool_t ThreadComPort_SendMsg(uint8_t Comand, void *Data, uint8_t DataLen)
{

	if((Data == NULL) && DataLen)
      return false;

    uint8_t *buf = ((uint8_t *)txBuf);


    buf[0] = SFD;
    buf[1] = DataLen+1;
    buf[2] = Comand;
    (void)memcpy(&buf[3], Data, DataLen);
    buf[3+DataLen] = EOFCOM;

    txCantBytes = 4 + DataLen;
    txBufIdx = 0;

    if(0 == WriteBytes(buf,txCantBytes))
    	Error_Handler();


    return true;

}


void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = BAUDRATE;
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

	switch(rxStt)
    {
    case PARSER_PRINC:						//Verifico el estado de la FSM de recepción
      if(rxBuffer[rxbfrcant] == SFD)		//Los paquetes de informacion deben tener la
      {										// ! SFD ! CANTIDAD DE DATOS ! DATOS ! EOFM !
    	  rxbfrcant++;						//Si el formato es correcto envio los datos al
    	  rxStt = PARSER_LENGTH;			//Layer superior del protocolo, si no descarto
    	  NxtRxCant = 1;					//e inicio nuevamente
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
      if(rxBuffer[rxbfrcant] == EOFCOM)
      {
    	  HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
       	  ThreadComPort_RxMsg(rxBuffer[2],rxBuffer+3,rxBuffer[1]);
      }
      rxStt = PARSER_PRINC;
	  NxtRxCant = 1;
	  rxbfrcant =0;
    break;
    default:
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
static int32_t WriteBytes(void *Buffer, int32_t n) //Escritura de n en el buffer de entrada
{
	if(HAL_OK == HAL_UART_Transmit_IT(&huart2, (const uint8_t *)Buffer,n))
			return 1;
	return 0;
}



