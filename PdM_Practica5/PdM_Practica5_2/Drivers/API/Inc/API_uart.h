/*
 * API_uart.h
 *
 *  Created on: Nov 21, 2023
 *      Author: martin
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#include "main.h"

/**********************************************************************
 * uartInit Function :
 * Function that initialize the UART periferial, and send the initialization
 * parameter through the port.
 *********************************************************************/
bool_t uartInit();

/**********************************************************************
* uartSendString Function:
* Send through the UART port a complete string.
* pstring must be NULL terminated to recognize the end.
*********************************************************************/
void uartSendString(uint8_t * pstring);

/**********************************************************************
* uartSendString Function:
* Send through the UART port the elements pointed for *pstring.
* @pstring is a pointer to the first element to be send.
* @size the amount of element to be send.
*********************************************************************/
void uartSendStringSize(uint8_t * pstring, uint16_t size);

/**********************************************************************
* uartReceiveStringSize Function:
* Read an specific amount of elements in the UART port buffer,
* and transfer them to the direction pointed by *pstring..
* @pstring is a pointer to the memory site where the first element will be save.
* @size the amount of element to be read.
*********************************************************************/
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);


#endif /* API_INC_API_UART_H_ */
