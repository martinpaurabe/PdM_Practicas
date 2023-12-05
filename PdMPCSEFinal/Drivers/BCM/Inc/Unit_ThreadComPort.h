#ifndef Unit_ThreadComPortH
#define Unit_ThreadComPortH
//---------------------------------------------------------------------------

//My Libraries
#include "API_uart.h"
#include "main.h" //Just to toggle the LED on debbugging state
#include "stm32f4xx_hal.h"


//--------------------------------------------------------------------------
#define ADQ_TIMEOUT 100 //Micro Segundos
#define SEC_TO_DOUBLE(T) ((T)/86400.0) //T: Tiempo en segundos


#define ERR_PUERTO 0x01
#define TIMEOUT    0x02
//---------------------------------------------------------------------------

extern TThreadComPort ThreadComPort;
//---------------------------------------------------------------------------
void ThreadComPort_Init(void);
//void ThreadComPort_Update(void);
bool_t ThreadComPort_SendMsg(BYTE Comand, void *Data, uint8_t DataLen);
extern void sciDataReceived(BYTE *buf);

#endif
