#ifndef Unit_ThreadComPortH
#define Unit_ThreadComPortH
//---------------------------------------------------------------------------
#include <time.h>


//My Libraries
#include "Unit_ComPort.h"


//---------------------------------------------------------------------------


#define DIM_ADQ 10000
//---------------------------------------------------------------------------

#define ERR_PUERTO 0x01
#define TIMEOUT    0x02

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
