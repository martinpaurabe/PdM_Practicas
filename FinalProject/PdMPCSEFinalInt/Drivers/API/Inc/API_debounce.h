/*
 * API_debounce.h
 *
 *  Created on: Nov 9, 2023
 *      Author: Martin Paura
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#include "API_delay.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "main.h"

#define FSM_PERIODE  10 	//Time between FSM updates in miliseconds

/**********************************************************************
 * debounceFSM_init Function:
 * Function that initialize the Finit State Machine that controls the
 * debouncing logic for the user button on de NUCLEO-F401RE.
 *********************************************************************/

void debounceFSM_init();

/**********************************************************************
 * debounceFSM_update Function:
 * Function that analize the events related with the user button, through
 * the Finit State Machine.
 *********************************************************************/

void debounceFSM_update();

/**********************************************************************
 * readKeyPosEdge Function:
 * This function sends true if there was a positive edge since the last
 * time it was called, false otherwise.
 *********************************************************************/
bool_t readKeyPosEdge();

/**********************************************************************
 * readKeyRelease Function:
 * This function sends true if there was a negative edge since the last
 * time it was called, false otherwise.
 *********************************************************************/
bool_t readKeyNegEdge();


#endif /* API_INC_API_DEBOUNCE_H_ */
