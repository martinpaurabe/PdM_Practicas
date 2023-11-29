#pragma hdrstop

#include "Unit_PuertoCom.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

static HANDLE m_hCom = NULL; //Handle al comm port.
static DCB m_dcb; // un DCB (estructura de Win)para configurar puertos
static bool m_CommOpen = false;
static int PuertoNum = 0;


void __fastcall OpenCommPort(unsigned int Baudios, int PortNum)
{
        AnsiString port = "COM";
        COMMTIMEOUTS m_timeouts;

        PuertoNum = PortNum;
        port += PortNum;

        if (m_CommOpen) CloseCommPort();//Si ya está abierto no lo abro de nuevo

        //Abro el puerto de comunicaciones
		m_hCom = CreateFileA( port.c_str(), //TEXT("COM4"),
						GENERIC_READ | GENERIC_WRITE,
                        0,    /* comm devices must be opened w/exclusive-access */
                        NULL, /* no security attrs */
                        OPEN_EXISTING, /* comm devices must use OPEN_EXISTING */
                        0,    /* not overlapped I/O */
                        NULL  /* hTemplate must be NULL for comm devices */
                        );

        // If CreateFile fails, throw an exception. CreateFile will fail if the
        // port is already open, or if the com port does not exist.
        if(m_hCom == INVALID_HANDLE_VALUE)
        {
          ShowMessage("             No se pudo abrir el puerto de comunicación             ");
          m_CommOpen = false;
          return;
        }

        if(!GetCommState(m_hCom, &m_dcb))
        {
          ShowMessage("No se pudo leer el estado del puerto");
          CloseHandle(m_hCom);
          m_CommOpen = false;
          return;
        }

        // dcb tiene los param actuales, metemos los nuestros
        m_dcb.BaudRate  = Baudios;
        m_dcb.ByteSize  = 8; //Cantidad de bits de dato
        m_dcb.Parity    = NOPARITY; //defined in windows.h
        m_dcb.fParity = false; //Chequeo de paridad
        m_dcb.StopBits  = ONESTOPBIT; //defined in windows.h

        // now we can set the properties of the port with our settings.
        if(!SetCommState(m_hCom,&m_dcb))
        {
          ShowMessage("No se pudo modificar el estado del puerto");
          CloseHandle(m_hCom);
          m_CommOpen = false;
          return;
        }

        m_timeouts.ReadTotalTimeoutConstant = 250;
        m_timeouts.ReadIntervalTimeout = MAXDWORD;
        m_timeouts.ReadTotalTimeoutMultiplier = MAXDWORD; //ver API WIN32
        if(!SetCommTimeouts(m_hCom,&m_timeouts))
        {
          ShowMessage("Error escribiendo los timeouts del puerto");
          CloseHandle(m_hCom);
          m_CommOpen = false;
          return;
        }

        if(!SetupComm(m_hCom,65536,4096))
        {
          ShowMessage("Error configurando los Buffers");
          CloseHandle(m_hCom);
          m_CommOpen = false;
          return;
        }

        m_CommOpen = true;

        Sleep(100);
}
//---------------------------------------------------------------------------


void __fastcall CloseCommPort(void)
{
    if(!m_CommOpen)       // if already closed, return
      return;

    if(CloseHandle(m_hCom) != 0) // CloseHandle is non-zero on success
        m_CommOpen = false;
    else
        ShowMessage("Error, no se pudo cerrar el puerto");
}
//---------------------------------------------------------------------------


bool ComAbierto(void)
{
  return m_CommOpen;
}
//---------------------------------------------------------------------------


int ComNum(void)
{
  return PuertoNum;
}
//---------------------------------------------------------------------------


void __fastcall SetBaudRate(unsigned int baud)
{
    unsigned int oldBaudRate = m_dcb.BaudRate; // backup
    m_dcb.BaudRate = baud;

    if(m_CommOpen) //Si el puerto está abierto actualizar baud rate
    {
        if(!SetCommState(m_hCom,&m_dcb))   // trato de setear
        {                                      // si no
            m_dcb.BaudRate = oldBaudRate;        // vuelvo al viejo B.rate
            ShowMessage("Error no se pudo cambiar el BaudRate");
        }
    }
}
//---------------------------------------------------------------------------


int BytesDisponibles(void)
{
    COMSTAT Status;
    DWORD Errores;

    if(!ClearCommError(m_hCom, &Errores, &Status))
      return -1;

    return Status.cbInQue; //Return bytes in input buffer
}
//---------------------------------------------------------------------------


DWORD ComError(void)
{
    COMSTAT Status;
    DWORD Errores;

    if(!ClearCommError(m_hCom, &Errores, &Status))
      return -1;

    return Errores;
}
//---------------------------------------------------------------------------


int GetByte(BYTE *value)
{
    unsigned long BytesRead;

    if (!m_CommOpen)
      return 0;

    if(!ReadFile(m_hCom, value, 1, &BytesRead, NULL))
      return -1;

    return BytesRead;
}
//---------------------------------------------------------------------------


int ReadBytes(void *Buffer, int n)
{
    unsigned long BytesRead;

    if (!m_CommOpen) //Verifica si el puerto esta abierto
      return 0;

    if(!ReadFile(m_hCom, Buffer, n, &BytesRead, NULL))
      return -1;

    return BytesRead;
}
//---------------------------------------------------------------------------


int PutByte(BYTE value)
{
    unsigned long BytesWritten;

    if (!m_CommOpen)
      return 0;

    if(!WriteFile(m_hCom, &value, 1, &BytesWritten, NULL))
      return -1;

    return BytesWritten;
}
//---------------------------------------------------------------------------


int WriteBytes(void *Buffer, int n)
{
    unsigned long BytesWritten;

    if (!m_CommOpen) //Verifica si el puerto esta abierto
      return 0;

    if(!WriteFile(m_hCom, Buffer, n, &BytesWritten, NULL))
      return -1;

    return BytesWritten;
}
//---------------------------------------------------------------------------

bool sendSciMsg(byte codigo, void *data, DWORD dataLen)
{
    DWORD cantWritten = 0;

    if(!m_CommOpen) //Verifica si el puerto esta abierto
      return false;

    if((data == NULL) && dataLen)
      return false;

    cantWritten += PutByte(SFD);
    cantWritten += PutByte(dataLen+1);
    cantWritten += PutByte(codigo);
    if(!WriteFile(m_hCom, data, dataLen, &cantWritten, NULL))
      return false;
    cantWritten += PutByte(EOF);

    if(cantWritten < (4+dataLen))
    {
      return false;
    }

    return true;
}
//---------------------------------------------------------------------------

