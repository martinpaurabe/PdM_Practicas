#include <vcl.h>
#pragma hdrstop
#include "Unit_ThreadComPort.h"
#pragma package(smart_init)

#include "Unit_PuertoCom.h"
//---------------------------------------------------------------------------

#define ADQ_TIMEOUT .1 //Segundos
#define SEC_TO_DOUBLE(T) ((T)/86400.0) //T: Tiempo en segundos

enum {PARSER_PRINC, PARSER_LENGTH, PARSER_DATA, PARSER_EOF};

TThreadComPort *ThreadComPort;
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall ThreadComPort::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TThreadComPort::TThreadComPort(bool CreateSuspended)
        : TThread(CreateSuspended)
{
  OnTerminate = &OnTerminateFc;
//  Priority = tpTimeCritical;
  FreeOnTerminate = false;

  rxParser = PARSER_PRINC;
}
//---------------------------------------------------------------------------

void TThreadComPort()
{
  DWORD CantBytesReadAnt;
  byte Dato;

  while(!Terminated)
  {
    Sleep((unsigned int)1);

    for(int i = 0; i < 10; i++)
    {
      ComErr = ComError();
      CantBytesReadAnt = CantBytesRead;
      CantBytesRead = BytesDisponibles();
      if(CantBytesRead > DIM_ADQ)
      {
        CloseCommPort();
      }
      if((rxParser == PARSER_PRINC) || (CantBytesRead - CantBytesReadAnt))
      {
        Tiempo = Tiempo.CurrentDateTime();
      }
      if((Tiempo.CurrentDateTime() - Tiempo).operator double() >= SEC_TO_DOUBLE(ADQ_TIMEOUT))
      {
        EstSciRv |= TIMEOUT;
        rxParser = PARSER_PRINC;
      }

      if(CantBytesRead > 0)
      {
        switch(rxParser)
        {
        case PARSER_PRINC:
          ReadBytes(&Dato, 1);
          if(Dato == SFD)
          {
            rxParser = PARSER_LENGTH;
          }
        break;
        case PARSER_LENGTH:
          ReadBytes(&rxCantBytes, 1);
          rxParser = PARSER_DATA;

        break;
        case PARSER_DATA:
          if(CantBytesRead >= rxCantBytes)
          {
            ReadBytes(rxBuf, rxCantBytes);
            rxParser = PARSER_EOF;
          }

        break;
        case PARSER_EOF:
          ReadBytes(&Dato, 1);
          if(Dato == EOF)
          {
            sciDataReceived(rxBuf);
          }
          rxParser = PARSER_PRINC;

        break;
        }
        EstSciRv &= ~ERR_PUERTO;
      }
      else if(CantBytesRead == 0)
      {
        EstSciRv &= ~ERR_PUERTO;
      }
      else
      {
        EstSciRv |= ERR_PUERTO;
      }
    }
  }
}
//---------------------------------------------------------------------------

// Al terminar la tarea
void __fastcall TThreadComPort::OnTerminateFc(TObject *Sender)
{
//  ShowMessage("          Captura interrumpida          ");
}
//---------------------------------------------------------------------------

