#include "Unit_ThreadComPort.h"
#include "Unit_ComPort.h"
#include "Unit_Disp.h"
//---------------------------------------------------------------------------
#define ADQ_TIMEOUT 100 //Micro Segundos
#define SEC_TO_DOUBLE(T) ((T)/86400.0) //T: Tiempo en segundos

#define DIM_ADQ 10000
//---------------------------------------------------------------------------

#define ERR_PUERTO 0x01
#define TIMEOUT    0x02

enum {PARSER_PRINC, PARSER_LENGTH, PARSER_DATA, PARSER_EOF};

static TThreadComPort ThreadComPort;
//---------------------------------------------------------------------------


void ThreadComPort_Init(void)
{
  if(0 == OpenCommPort(115200))

  ThreadComPort.rxParser = PARSER_PRINC;
}
//---------------------------------------------------------------------------

void ThreadComPort_Update(void)
{
  DWORD CantBytesReadAnt;
  BYTE Dato;

  while(1)
  {

    for(int i = 0; i < 10; i++)
    {
      ThreadComPort.ComErr = ComError();
      CantBytesReadAnt = ThreadComPort.CantBytesRead;
      ThreadComPort.CantBytesRead = BytesDisponibles();
      if(ThreadComPort.CantBytesRead  > DIM_ADQ)
      {
        CloseCommPort();
      }
      if((ThreadComPort.rxParser == PARSER_PRINC) || (ThreadComPort.CantBytesRead  - CantBytesReadAnt))
      {
        ThreadComPort.Tiempo = time(NULL);
      }
      if(difftime(time(NULL),ThreadComPort.Tiempo)*1000 >= ADQ_TIMEOUT)
      {
        ThreadComPort.EstSciRv |= TIMEOUT;
        ThreadComPort.rxParser = PARSER_PRINC;
      }

      if(ThreadComPort.CantBytesRead  > 0)
      {
        printf("Llegó dato\r\n");
        switch(ThreadComPort.rxParser)
        {
        case PARSER_PRINC:
          ReadBytes(&Dato, 1);
          if(Dato == SFD)
          {
            ThreadComPort.rxParser = PARSER_LENGTH;
          }
        break;
        case PARSER_LENGTH:
          ReadBytes(&ThreadComPort.rxCantBytes, 1);
          ThreadComPort.rxParser = PARSER_DATA;

        break;
        case PARSER_DATA:
          if(ThreadComPort.CantBytesRead  >= ThreadComPort.rxCantBytes)
          {
            ReadBytes(ThreadComPort.rxBuf, ThreadComPort.rxCantBytes);
            ThreadComPort.rxParser = PARSER_EOF;
          }

        break;
        case PARSER_EOF:
          ReadBytes(&Dato, 1);
          if(Dato == EOF)
          {
            sciDataReceived(ThreadComPort.rxBuf);
          }
          ThreadComPort.rxParser = PARSER_PRINC;

        break;
        }
        ThreadComPort.EstSciRv &= ~ERR_PUERTO;
      }
      else if(ThreadComPort.CantBytesRead  == 0)
      {
        ThreadComPort.EstSciRv &= ~ERR_PUERTO;
      }
      else
      {
        ThreadComPort.EstSciRv |= ERR_PUERTO;
      }
    }
  }
}
//---------------------------------------------------------------------------

void ThreadComPort_End(void)
{
  CloseCommPort();
  ThreadComPort.rxParser = PARSER_PRINC;
}
//---------------------------------------------------------------------------

