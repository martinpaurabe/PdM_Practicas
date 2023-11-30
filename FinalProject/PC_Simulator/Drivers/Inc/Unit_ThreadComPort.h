#ifndef Unit_ThreadComPortH
#define Unit_ThreadComPortH
//---------------------------------------------------------------------------

// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

//---------------------------------------------------------------------------

/*
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
*/
#endif
