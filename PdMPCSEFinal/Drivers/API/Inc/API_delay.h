/**********************************************************************************************
 * API_delay.h
 *
 *  Created on: Nov 02, 2023
 *      Author: Verónica anda Javier and Martin
 **********************************************************************************************/
#ifndef API_DELAY_H_
#define API_DELAY_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#include "Def_TipoDatos.h"


typedef uint32_t tick_t; // Qué biblioteca se debe incluir para que esto compile? stdin.h
typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;


/**********************************************************************
 * delayInit Function:
 * Receive a pointer to a delay_t type and initialize it with the
 * time duration specify in the duration parameter in usec.
 * Do not initialize the counter and leave the running state on false.
 *********************************************************************/

void delayInit( delay_t * delay, tick_t duration );

/**********************************************************************
 * delayRead Function:
 * This function is used to start the time counter or to verify if the
 * timer has finished.
 * Receive a pointer to a delay_t type to be analized or that we want to launch counter.
 * Send back true value if the time has been completed or false otherwise
 * (if the time has not been arrived or just start to run)
 *********************************************************************/

bool_t delayRead( delay_t * delay );

/**********************************************************************
 * delayWrite Function:
 * This function is used to set the duration time in usec of a delay structure
 *********************************************************************/
void delayWrite( delay_t * delay, tick_t duration );


#endif /* API_DELAY_H_ */
