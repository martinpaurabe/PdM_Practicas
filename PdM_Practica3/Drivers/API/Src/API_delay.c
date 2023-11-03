/**********************************************************
 * API_delay.c
 *
 *  Created on: 02/11, 2023
 *  Author: Veronica and Javier and Martin
 **********************************************************/
#include "API_delay.h"

/**********************************************************************
 * delayInit Function:
 * Receive a pointer to a delay_t type and initialize it with the
 * time duration specify in the duration parameter in usec.
 * Do not initialize the counter and leave the running state on false.
 *********************************************************************/

void delayInit( delay_t * delay, tick_t duration )
{
	delay->duration = duration;
	delay->running = false;
}

/**********************************************************************
 * delayRead Function:
 * This function is used to start the time counter or to verify if the
 * timer has finished.
 * Receive a pointer to a delay_t type to be analized or that we want to launch counter.
 * Send back true value if the time has been completed or false otherwise
 * (if the time has not been arrived or just start to run)
 *********************************************************************/

bool_t delayRead( delay_t * delay )
{

	bool_t TimeArrived = false;
	if(delay->running)
	{
		if(HAL_GetTick()-delay->startTime >= delay->duration)
		{
			TimeArrived = true;
			delay->running = false;
		}
	}
	else
	{
		delay->startTime = HAL_GetTick();
		delay->running = true;
	}

	return TimeArrived;
}
/**********************************************************************
 * delayWrite Function:
 * This function is used to set the duration time in usec of a delay structure
 *********************************************************************/

void delayWrite( delay_t * delay, tick_t duration )
{
	delay->duration = duration;
}


