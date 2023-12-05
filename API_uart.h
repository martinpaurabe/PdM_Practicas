/*
 * API_uart.h
 *
 *  Created on: Nov 21, 2023
 *      Author: martin
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

//#include "main.h"
#include "stm32f4xx_hal.h"



//Define my types
#define BYTE uint8_t
#define DWORD uint16_t

//#define BAUDRATE 921600
#define BAUDRATE 115200
#define DIM_ADQ 100

#define SFD     '#'
#define EOFCOM  255
//---------------------------------------------------------------------------
/**********************************************************************
 * OpenCommPort Function :
 * Function that initialize the UART periferial, and send the initialization
 * parameter through the port.
 * It devolve One if the port was opened correctly and zero otherwise.
 *********************************************************************/
int32_t OpenCommPort(uint32_t Baudios); //Abre la comunicacion

/**********************************************************************
*ComOpened Function:
*Devolve true if there is a opened com port, false otherwise*
***********************************************************************/
void CloseCommPort(void); //Cierra la comunicacion

/**********************************************************************
*ComOpened Function:
*Devolve true if there is a opened com port, false otherwise*
***********************************************************************/

bool_t ComOpened(void);


int32_t ComNum(void); //Devuelve el numero del puerto COM abierto
int32_t BytesDisponibles(void); //Devuelve la cantidad de Bytes en el buffer de entrada
DWORD ComError(void); //Devuelve en una variable de 32 bits los errores del puerto. Luego los flags de error del dispositivo son borrados.

int32_t GetByte(BYTE *value); //Lectura un caracter
int32_t ReadBytes(void *Buffer, int32_t n); //Lectura de n del buffer de entrada
int32_t PutByte(BYTE value); //Envio de un caracter




#endif /* API_INC_API_UART_H_ */
