#ifndef Unit_PuertoComH
#define Unit_PuertoComH
//---------------------------------------------------------------------------


//#define BAUDRATE 921600
#define BAUDRATE 115200

#define SFD  '#'
#define EOF  VK_RETURN
//---------------------------------------------------------------------------

void __fastcall OpenCommPort(unsigned int Baudios, int PortNum); //Abre la comunicacion
void __fastcall CloseCommPort(void); //Cierra la comunicacion

bool ComAbierto(void); //Devuelve 1 si hay un puerto abierto
int ComNum(void); //Devuelve el numero del puerto COM abierto
int BytesDisponibles(void); //Devuelve la cantidad de Bytes en el buffer de entrada
DWORD ComError(void); //Devuelve en una variable de 32 bits los errores del puerto. Luego los flags de error del dispositivo son borrados.

int GetByte(BYTE *value); //Lectura un caracter
int ReadBytes(void *Buffer, int n); //Lectura de n del buffer de entrada
int PutByte(BYTE value); //Envio de un caracter
int WriteBytes(void *Buffer, int n); //Escritura de n en el buffer de entrada
bool sendSciMsg(byte codigo, void *data, DWORD dataLen);

//---------------------------------------------------------------------------
#endif
