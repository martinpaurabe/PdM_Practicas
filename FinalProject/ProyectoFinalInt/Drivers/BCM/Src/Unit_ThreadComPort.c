#include "Unit_ThreadComPort.h"



enum {PARSER_PRINC, PARSER_LENGTH, PARSER_DATA, PARSER_EOF};

static TThreadComPort ThreadComPort;
//---------------------------------------------------------------------------

void ThreadComPort_Init(void)
{
  OpenCommPort(115200);
  ThreadComPort.rxParser = PARSER_PRINC;
}
//---------------------------------------------------------------------------

void ThreadComPort_Update(void)
{
  DWORD CantBytesReadAnt;
  BYTE Dato;

  for(int i = 0; i < 20; i++)
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
*/
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
            HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
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

void ThreadComPort_End(void)
{
  CloseCommPort();
  ThreadComPort.rxParser = PARSER_PRINC;
}
//---------------------------------------------------------------------------