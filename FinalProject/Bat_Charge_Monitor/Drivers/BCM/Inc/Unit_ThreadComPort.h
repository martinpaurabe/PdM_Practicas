#ifndef Unit_ThreadComPortH
#define Unit_ThreadComPortH
//---------------------------------------------------------------------------

//My Libraries
#include "API_uart.h"
#include "Unit_Disp.h"


//--------------------------------------------------------------------------
#define ADQ_TIMEOUT 100 //Micro Segundos
#define SEC_TO_DOUBLE(T) ((T)/86400.0) //T: Tiempo en segundos

#define DIM_ADQ 100
//---------------------------------------------------------------------------

#define ERR_PUERTO 0x01
#define TIMEOUT    0x02
//---------------------------------------------------------------------------

typedef struct 
{
        DWORD ComErr;

        DWORD CantBytesRead;
        time_t Tiempo;
 
        BYTE EstSciRv;

        BYTE rxBuf[DIM_ADQ];
        DWORD rxCantBytes;
        BYTE rxParser;

}TThreadComPort;

//---------------------------------------------------------------------------
void ThreadComPort_Init(void);
void ThreadComPort_Update(void);


#endif
