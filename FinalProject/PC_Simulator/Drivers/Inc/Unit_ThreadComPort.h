#ifndef Unit_ThreadComPortH
#define Unit_ThreadComPortH
//---------------------------------------------------------------------------

#include <Classes.hpp>
//---------------------------------------------------------------------------


#define DIM_ADQ 10000
//---------------------------------------------------------------------------

#define ERR_PUERTO 0x01
#define TIMEOUT    0x02

class TThreadComPort : public TThread
{
private:
protected:
        void __fastcall Execute();
        void __fastcall OnTerminateFc(TObject *Sender);

public:
        __fastcall TThreadComPort(bool CreateSuspended);

        DWORD ComErr;

        DWORD CantBytesRead;
        TDateTime Tiempo;

        byte EstSciRv;

        byte rxBuf[DIM_ADQ];
        DWORD rxCantBytes;
        byte rxParser;

        void __fastcall sciDataReceived(byte *buf);
};

extern TThreadComPort *ThreadComPort;
//---------------------------------------------------------------------------
#endif
