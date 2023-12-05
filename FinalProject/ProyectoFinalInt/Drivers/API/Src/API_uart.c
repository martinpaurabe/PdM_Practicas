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



static volatile uint8_t rxParser;

volatile uint8_t *sciRxBuf;


//Tx
TThreadComPort ThreadComPort;

static UART_HandleTypeDef huart2;
//static int32_t serial_port = 0;
static bool_t m_CommOpen = false;

/**********************************************************************
 * OpenCommPort Function :
 * Function that initialize the UART periferial, and send the initialization
 * parameter through the port.
 * It devolve One if the port was opened correctly and zero otherwise.
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
	  ThreadComPort.rxParser = PARSER_PRINC;
	  GetByte(rxBuf1);
	  if (HAL_UART_Init(&huart2) != HAL_OK)
	  {
		  PortStatus=1;
	  }

	  return PortStatus;

}

/**********************************************************************
*CloseCommPort Function:
*Close the open port.*
*********************************************************************/

void CloseCommPort(void)
{
	HAL_UART_DeInit(&huart2);
	m_CommOpen =false;

}

/**********************************************************************
*ComOpened Function:
*Devolve true if there is a opened com port, false otherwise*
***********************************************************************/

bool_t ComOpened(void)
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
*ComError Function
*********************************************************************/

DWORD ComError(void)
{
	return (DWORD) huart2.ErrorCode;
}


/**********************************************************************
*GetByute Function:
*Send a byte through de com port
*********************************************************************/

int32_t GetByte(BYTE *value)
{
	if(HAL_OK == HAL_UART_Receive_IT(&huart2, value,1))
		return 1;
	return 0;
}


/**********************************************************************
********************************************************************/
int32_t ReadBytes(void *Buffer, int32_t n) //Lectura de n del buffer de entrada
{
	if(HAL_OK == HAL_UART_Receive_IT(&huart2, Buffer,n))
		return 1;
	return 0;
}

/**********************************************************************
* PutByte Function:
* Send through the UART port an element.
* @param value, byte to be send
* Send back a one if value was send correctly and zero otherwise
*********************************************************************/
int32_t PutByte(BYTE value)
{
	if(HAL_OK == HAL_UART_Transmit(&huart2, (const uint8_t *)&value, 1,1))
		return 1;
	return 0;

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

/**********************************************************************
* BytesDisponibles Function:
* Return the amount of bytes available. (It doesn´t work)
*********************************************************************/
int32_t BytesDisponibles(void)
{
	return (int32_t)  (huart2.RxXferCount-huart2.RxXferCount);
}


//---------------------------------------------------------------------------
uint8_t Dato;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  DWORD CantBytesReadAnt;

  HAL_UART_Receive_IT(&huart2, &Dato,1);

//  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

//  for(int i = 0; i < 20; i++)
  {
/*
      ThreadComPort.ComErr = ComError();
      CantBytesReadAnt = ThreadComPort.CantBytesRead;
      ThreadComPort.CantBytesRead = BytesDisponibles();
      if(ThreadComPort.CantBytesRead  > DIM_ADQ)
      {
        CloseCommPort();
      }
      if((ThreadComPort.rxParser == PARSER_PRINC) || (ThreadComPort.CantBytesRead  - CantBytesReadAnt))
      {
        ThreadComPort.Tiempo = HAL_GetTick();
      }
      if(HAL_GetTick()-ThreadComPort.Tiempo >= ADQ_TIMEOUT)
      {
        ThreadComPort.EstSciRv |= TIMEOUT;
        ThreadComPort.rxParser = PARSER_PRINC;
      }

      if(ThreadComPort.CantBytesRead  > 0)
      {
	  switch(ThreadComPort.rxParser)
        {
        case PARSER_PRINC:
  		  GetByte(&Dato);
          if(Dato == SFD)
          {
            ThreadComPort.rxParser = PARSER_LENGTH;
          }
        break;
        case PARSER_LENGTH:
          GetByte(&Dato);
       	  ThreadComPort.rxCantBytes = Dato;
       	  ThreadComPort.rxParser = PARSER_DATA;
       break;
        case PARSER_DATA:
          if(1 == ReadBytes(ThreadComPort.rxBuf, ThreadComPort.rxCantBytes))
           	ThreadComPort.rxParser = PARSER_EOF;
          else
            ThreadComPort.rxParser = PARSER_PRINC;
        break;
        case PARSER_EOF:
          ReadBytes(&Dato, 1);
          if(Dato == EOFCOM)
          {
            sciDataReceived(ThreadComPort.rxBuf);
          }
          ThreadComPort.rxParser = PARSER_PRINC;

        break;
        }

	  /*ThreadComPort.EstSciRv &= ~ERR_PUERTO;
      }
      else if(ThreadComPort.CantBytesRead  == 0)
      {
        ThreadComPort.EstSciRv &= ~ERR_PUERTO;
      }
      else
      {
        ThreadComPort.EstSciRv |= ERR_PUERTO;
      }

   }*/
  }
}
//---------------------------------------------------------------------------

