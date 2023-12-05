/*
 * Unit_ThredComPort.h
 *
 *  Created on: Dec 3, 2023
 *      Author: martin
 */
#ifndef Unit_ThreadComPortH
#define Unit_ThreadComPortH
//---------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>


#include "Def_TipoDatos.h"

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
bool_t ThreadComPort_SendMsg(uint8_t Comand, void *Data, uint8_t DataLen);
extern void ThreadComPort_RxMsg(uint8_t Comand, uint8_t *Data, uint8_t DataLen);


#endif
