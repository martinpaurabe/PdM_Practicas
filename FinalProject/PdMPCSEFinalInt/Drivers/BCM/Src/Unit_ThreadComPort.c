#include "Unit_ThreadComPort.h"


static uint8_t txBuf[DIM_ADQ];
static volatile uint8_t txBufIdx;
static uint8_t txCantBytes; //Cantidad de datos a transmitir en buffer



void ThreadComPort_Init(void)
{
	MX_USART2_UART_Init();
//  OpenCommPort(115200);
//  set_rxCallBack(ThreadComPort_Update);
//  ThreadComPort_Update();
//  GetByte(txBuf);
}


/**********************************************************************
* BytesDisponibles Function:
* Return the amount of
*********************************************************************/

bool ThreadComPort_SendMsg(BYTE Comand, void *Data, uint8_t DataLen)
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


