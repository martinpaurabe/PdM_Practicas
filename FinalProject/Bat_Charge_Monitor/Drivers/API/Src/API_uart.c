/*
 * API_uart.c
 *
 *  Created on: Nov 21, 2023
 *      Author: martin
 */
#include <API_uart.h>

static UART_HandleTypeDef huart2;
//static int32_t serial_port = 0;
static bool_t m_CommOpen = false;

/**********************************************************************
 * uartInit Function :
 * Function that initialize the UART periferial, and send the initialization
 * parameter through the port.
 * It devolve TRUE if the port is initialized correctly and false otherwise.
 *********************************************************************/
int32_t OpenCommPort(uint32_t Baudios)
{

	  int32_t PortStatus=0;

	  huart2.Instance = USART2;								//Initializing the UART periferial port
	  huart2.Init.BaudRate = 115200;
	  huart2.Init.WordLength = UART_WORDLENGTH_8B;
	  huart2.Init.StopBits = UART_STOPBITS_1;
	  huart2.Init.Parity = UART_PARITY_NONE;
	  huart2.Init.Mode = UART_MODE_TX_RX;
	  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	  huart2.RxXferSize = 256;
	  huart2.TxXferSize = 256;
	  if (HAL_UART_Init(&huart2) != HAL_OK)
	  {
		  PortStatus=1;
	  }
	  else
	  {
		  char BaudRateMSG[]="Communication Init \n\rBaudrate: ";
		  while(HAL_BUSY == HAL_UART_Transmit(&huart2, (const uint8_t *) BaudRateMSG, sizeof(BaudRateMSG),100));

		  char BaudRateValue[20];
		  int32_t BRValue=huart2.Init.BaudRate;
		  uint8_t i=0;
		  i=sprintf(BaudRateValue, "%d",(int)BRValue);   //Convert Baudrate value to string to show it on Std IO
		  BaudRateValue[i++] = 0x0A;
		  BaudRateValue[i++] = 0x0D;
		  BaudRateValue[i++] = 0x00;
		  while(HAL_BUSY == HAL_UART_Transmit(&huart2, (const uint8_t *) BaudRateValue, i,100));
		  m_CommOpen =true;
	  }

	  return PortStatus;

}

/**********************************************************************
*********************************************************************/

void CloseCommPort(void)
{
	HAL_UART_DeInit(&huart2);
	m_CommOpen =false;

}

/**********************************************************************
*********************************************************************/

bool_t ComAbierto(void)
{
	bool_t PortState = false;

	if(HAL_UART_STATE_RESET != huart2.gState)
		PortState = true;

	return PortState;

}

/**********************************************************************
*********************************************************************/

int32_t ComNum(void)
{
	return 2;
	//huart2.Instance
}

/**********************************************************************
*********************************************************************/

DWORD ComError(void)
{
	return (DWORD) huart2.ErrorCode;
}


/**********************************************************************
*********************************************************************/

int32_t GetByte(BYTE *value)
{
	return (int32_t) HAL_UART_Receive(&huart2, value, 1, 10);
}


/**********************************************************************
********************************************************************/
int32_t ReadBytes(void *Buffer, int32_t n) //Lectura de n del buffer de entrada
{
	if(HAL_OK == HAL_UART_Transmit(&huart2, (const uint8_t *)Buffer, n,100))
		return 1;
	return 0;
}

/**********************************************************************
* uartSendString Function:
* Send through the UART port the elements pointed for *pstring.
* @param pstring is a pointer to the first element to be send.
* @param size the amount of element to be send.
*********************************************************************/
int32_t PutByte(BYTE value) //Envio de un caracter
{
	if(HAL_OK == HAL_UART_Transmit(&huart2, (const uint8_t *)&value, 1,10))
		return 1;
	return 0;

}
/**********************************************************************
*********************************************************************/
int32_t WriteBytes(void *Buffer, int32_t n) //Escritura de n en el buffer de entrada
{
	if(HAL_OK == HAL_UART_Transmit(&huart2, (const uint8_t *)Buffer,n,10*n))
		return 1;
	return 0;
}

/**********************************************************************
* BytesDisponibles Function:
* Return the amount of
*********************************************************************/
int32_t BytesDisponibles(void) //Devuelve la cantidad de Bytes en el buffer de entrada
{
	return (int32_t) huart2.RxXferCount;
}

/**********************************************************************
* BytesDisponibles Function:
* Return the amount of
*********************************************************************/

bool sendSciMsg(BYTE codigo, void *data, uint8_t dataLen)
{
    DWORD cantWritten = 0;

    if(!m_CommOpen) //Verifica si el puerto esta abierto
      return false;

    if((data == NULL) && dataLen)
      return false;

    cantWritten += PutByte(SFD);
    cantWritten += PutByte(dataLen+1);
    cantWritten += PutByte(codigo);
    if(!WriteBytes(data,dataLen))
      return false;
    cantWritten += PutByte(EOFCOM);

    if(cantWritten < (4+dataLen))
    {
      return false;
    }

    return true;

}


