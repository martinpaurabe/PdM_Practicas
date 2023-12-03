/*
 * API_uart.c
 *
 *  Created on: Nov 21, 2023
 *      Author: martin
 */
#include <API_uart.h>

//Rx
static volatile uint8_t rxBuf1[DIM_ADQ];
static volatile uint8_t rxBuf2[DIM_ADQ];
static volatile uint8_t rxBufIdx;
static volatile uint8_t rxCantBytes;

enum {PARSER_PRINC, PARSER_LENGTH, PARSER_DATA, PARSER_EOF};
static volatile uint8_t rxParser;

volatile uint8_t *sciRxBuf;


//Tx
static uint8_t txBuf[DIM_ADQ];
static volatile uint8_t txBufIdx;
static uint8_t txCantBytes; //Cantidad de datos a transmitir en buffer

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
	  huart2.RxXferSize = DIM_ADQ;
	  huart2.TxXferSize = DIM_ADQ;
	  if (HAL_UART_Init(&huart2) != HAL_OK)
	  {
		  PortStatus=1;
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
	if(HAL_OK == HAL_UART_Receive(&huart2, (uint8_t *)value, 1,100))
		return 1;
	return 0;
}


/**********************************************************************
********************************************************************/
int32_t ReadBytes(void *Buffer, int32_t n) //Lectura de n del buffer de entrada
{
	if(HAL_OK == HAL_UART_Receive(&huart2, (uint8_t *)Buffer, n,n*100))
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
	if(HAL_OK == HAL_UART_Transmit(&huart2, (const uint8_t *)Buffer,n,n*10))
		return 1;
	return 0;
}

/**********************************************************************
* BytesDisponibles Function:
* Return the amount of
*********************************************************************/
int32_t BytesDisponibles(void) //Devuelve la cantidad de Bytes en el buffer de entrada
{
	return (int32_t)  (huart2.RxXferCount-huart2.RxXferCount);
}

/**********************************************************************
* BytesDisponibles Function:
* Return the amount of
*********************************************************************/

bool sendSciMsg(BYTE Comand, void *Data, uint8_t DataLen)
{
//    DWORD cantWritten = 0;

/*
     if(!m_CommOpen) //Verifica si el puerto esta abierto
      return false;
*/

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

    WriteBytes(buf,txCantBytes);

    return true;

}


