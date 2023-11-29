/*
 * API_uart.c
 *
 *  Created on: Nov 21, 2023
 *      Author: martin
 */
#include <API_uart.h>

static UART_HandleTypeDef huart2;

/**********************************************************************
 * uartInit Function :
 * Function that initialize the UART periferial, and send the initialization
 * parameter through the port.
 * It devolve TRUE if the port is initialized correctly and false otherwise.
 *********************************************************************/
bool_t uartInit()
{

	  bool_t PortStatus=true;

	  huart2.Instance = USART2;								//Initializing the UART periferial port
	  huart2.Init.BaudRate = 115200;
	  huart2.Init.WordLength = UART_WORDLENGTH_8B;
	  huart2.Init.StopBits = UART_STOPBITS_1;
	  huart2.Init.Parity = UART_PARITY_NONE;
	  huart2.Init.Mode = UART_MODE_TX_RX;
	  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	  if (HAL_UART_Init(&huart2) != HAL_OK)
	  {
		  PortStatus=true;
	  }
	  else
	  {
		  char BaudRateMSG[]="Communication Init \n\rBaudrate: ";
		  while(HAL_BUSY == HAL_UART_Transmit(&huart2, (const uint8_t *) BaudRateMSG, sizeof(BaudRateMSG),100));

		  char BaudRateValue[20];
		  int32_t BRValue=huart2.Init.BaudRate;
		  uint8_t i=0;
		  i=sprintf(BaudRateValue, "%d",BRValue);   //Convert Baudrate value to string to show it on Std IO
		  BaudRateValue[i++] = 0x0A;
		  BaudRateValue[i++] = 0x0D;
		  BaudRateValue[i++] = 0x00;
		  while(HAL_BUSY == HAL_UART_Transmit(&huart2, (const uint8_t *) BaudRateValue, i,100));
	  }

	  return PortStatus;

}

/**********************************************************************
* uartSendString Function:
* Send through the UART port a complete string.
* pstring must be NULL terminated to recognize the end.
*********************************************************************/
void uartSendString(uint8_t * pstring)
{
	HAL_UART_Transmit(&huart2, (const uint8_t *)pstring, sizeof(pstring),100);
}

/**********************************************************************
* uartSendString Function:
* Send through the UART port the elements pointed for *pstring.
* @param pstring is a pointer to the first element to be send.
* @param size the amount of element to be send.
*********************************************************************/
void uartSendStringSize(uint8_t * pstring, uint16_t size)
{
	HAL_UART_Transmit(&huart2, (const uint8_t *)pstring, size,100);

}
/**********************************************************************
* uartReceiveStringSize Function:
* Read an specific amount of elements in the UART port buffer,
* and transfer them to the direction pointed by *pstring..
* @param pstring is a pointer to the memory site where the first element will be save.
* @param size the amount of element to be read.
*********************************************************************/
void uartReceiveStringSize(uint8_t * pstring, uint16_t size)
{
	HAL_UART_Receive(&huart2,pstring, size, 100);
}



