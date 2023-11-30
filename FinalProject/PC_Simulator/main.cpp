//#include <iostream>
//#include <stdio.h>


//My Libraries
//#include "Drivers/Inc/Unit_ThreadComPort.h"
#include "Drivers/Unit_ComPort.h"


using namespace std;

int main()
{
  OpenCommPort(115200);
  // Write to serial port
 // unsigned char msg[] = { 'H', 'e', 'l', 'l', 'o', '\r' };
  

  // Allocate memory for read buffer, set size according to your needs
  char read_buf [256];

  // Normally you wouldn't do this memset() call, but since we will just receive
  // ASCII data for this example, we'll set everything to 0 so we can
  // call printf() easily.
  memset(&read_buf, '\0', sizeof(read_buf));
  int num_bytes=0;

  while(1)
  {
   // Read bytes. The behaviour of read() (e.g. does it block?,
    // how long does it block for?) depends on the configuration
    // settings above, specifically VMIN and VTIME
    num_bytes = BytesDisponibles(); 
  
       // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
    if (num_bytes < 0) {
      printf("Error reading: %s", strerror(errno));
      return 1;
    }

    // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
    // print it to the screen like this!)
    
    if(10< num_bytes)
    {
      ReadBytes(read_buf,9);
      printf("%s\r\n", read_buf);
    }
 
  //cout << "Hello world!" << endl;
  }  
  
  return 0; // success
};



//---------------------------------------------------------------------------
//My Private Variables
// Create new termios struct, we call it 'tty' for convent2ion
static struct termios tty;
static int32_t serial_port = 0;
static bool m_CommOpen = false;

int32_t OpenCommPort(unsigned int Baudios)
{
  serial_port = open("/dev/ttyACM0", O_RDWR);

  

  // Read in existing settings, and handle any error
  if(tcgetattr(serial_port, &tty) != 0) {
    printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    return 1;
  }

  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
  tty.c_cflag |= CS8; // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE; // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
  tty.c_cc[VMIN] = 0;

  // Set in/out baud rate to be 115200
  cfsetispeed(&tty, B115200);
  cfsetospeed(&tty, B115200);

  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
    printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    return 1;
  }
  
  m_CommOpen = true;
  return 0;
}
//---------------------------------------------------------------------------


void CloseCommPort(void)
{
    if(!serial_port)       // if already closed, return
      return;

    if(close(serial_port) != 0) // CloseHandle is non-zero on success
      m_CommOpen = false;
    else
      printf("Error, no se pudo cerrar el puerto");
}
//---------------------------------------------------------------------------


bool ComAbierto(void)
{
  return m_CommOpen;
}
//---------------------------------------------------------------------------


int32_t ComNum(void)
{
  return serial_port;
}
//---------------------------------------------------------------------------

/*
void SetBaudRate(uint32_t baud)
{
    unsigned int oldBaudRate = m_dcb.BaudRate; // backup
    m_dcb.BaudRate = baud;

    if(m_CommOpen) //Si el puerto est� abierto actualizar baud rate
    {
        if(!SetCommState(m_hCom,&m_dcb))   // trato de setear
        {                                      // si no
            m_dcb.BaudRate = oldBaudRate;        // vuelvo al viejo B.rate
            ShowMessage("Error no se pudo cambiar el BaudRate");
        }
    }
}
*/
//---------------------------------------------------------------------------


int32_t BytesDisponibles(void)
{
  int32_t bytes_avaiable;
  ;
    if(-1 == ioctl(serial_port, FIONREAD, &bytes_avaiable))
      return -1;

    return bytes_avaiable; //Return bytes in input buffer
}
//---------------------------------------------------------------------------


DWORD ComError(void)
{
   // COMSTAT Status;
    DWORD Errores=0;

 //   if(!ClearCommError(m_hCom, &Errores, &Status))
 //     return -1;

    return Errores;
}
//---------------------------------------------------------------------------


int32_t GetByte(BYTE *value)
{
    unsigned long BytesRead;

    if (!m_CommOpen)
      return 0;

    if(!read(serial_port, value, 1))
      return -1;

    return BytesRead;
}
//---------------------------------------------------------------------------


int32_t ReadBytes(void *Buffer, int n)
{
    unsigned long BytesRead;

    if (!m_CommOpen) //Verifica si el puerto esta abierto
      return 0;

    if(!read(serial_port, Buffer, n))
      return -1;

    return BytesRead;
}
//---------------------------------------------------------------------------


int32_t PutByte(BYTE value)
{
    unsigned long BytesWritten;

    if (!m_CommOpen)
      return 0;

    if(!write(serial_port,  &value, 1))
      return -1;

    return BytesWritten;
}
//---------------------------------------------------------------------------


int32_t WriteBytes(void *Buffer, int n)
{
    unsigned long BytesWritten;

    if (!m_CommOpen) //Verifica si el puerto esta abierto
      return 0;
    if(!write(serial_port, Buffer, n))
      return -1;

    return BytesWritten;
}
//---------------------------------------------------------------------------

bool sendSciMsg(BYTE codigo, void *data, DWORD dataLen)
{
    DWORD cantWritten = 0;

    if(!m_CommOpen) //Verifica si el puerto esta abierto
      return false;

    if((data == NULL) && dataLen)
      return false;

    cantWritten += PutByte(SFD);
    cantWritten += PutByte(dataLen+1);
    cantWritten += PutByte(codigo);
    if(!write(serial_port, data, dataLen))
      return false;
    cantWritten += PutByte(EOF);

    if(cantWritten < (4+dataLen))
    {
      return false;
    }

    return true;
}
//---------------------------------------------------------------------------

