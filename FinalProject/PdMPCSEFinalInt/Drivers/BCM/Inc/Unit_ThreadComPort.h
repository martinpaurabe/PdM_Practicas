#ifndef Unit_ThreadComPortH
#define Unit_ThreadComPortH
//---------------------------------------------------------------------------

//My Libraries
//#include "API_uart.h"
#include "main.h" //Just to toggle the LED on debbugging state
#include "stm32f4xx_hal.h"

// C library headers
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#include "Def_TipoDatos.h"

//Define my types
#define BYTE uint8_t
#define DWORD uint16_t

//#define BAUDRATE 921600
#define BAUDRATE 115200
#define DIM_ADQ 100

#define SFD     '#'
#define EOFCOM  255

//--------------------------------------------------------------------------
#define ADQ_TIMEOUT 100 //Micro Segundos


//---------------------------------------------------------------------------

//extern TThreadComPort ThreadComPort;
//---------------------------------------------------------------------------
void ThreadComPort_Init(void);
bool_t ThreadComPort_SendMsg(BYTE Comand, void *Data, uint8_t DataLen);
extern void sciDataReceived(BYTE *buf);


#endif
