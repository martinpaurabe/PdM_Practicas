/*
 * API_debounce.c
 *
 *  Created on: Nov 9, 2023
 *      Author: Martin Paura
 */

#include "API_debounce.h"


#define DEBOUNCETIME 40;

void buttonPressed();		// it must turn on the LED
void buttonReleased();		// it must turn oFF the LED

//States of the FSM
typedef enum{
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RAISING,
} debounceState_t;

static debounceState_t debounceState;  //State variable of the FSM
static bool_t buttonEdge;			   //Variable that stores if there was or not a positive edge on the button position
static delay_t debounceDelay;		   //delay variable to manage the time on the FSM changes


void debounceFSM_init() {
	debounceState = BUTTON_UP;					//Initialize the state button like not pressed
	buttonEdge = false;							//No edge yet (no time)
	tick_t initialDelay = DEBOUNCETIME;         // Set the initial time of the delay (200ms)
	delayInit(&debounceDelay, initialDelay); 	// Initialize the counter

}



/**********************************************************************
 * debounceFSM_init Function:
 * Function that initialize the Finit State Machine that controls the
 * debouncing logic for the user button on de NUCLEO-F401RE.
 *********************************************************************/

void debounceFSM_update(){

	GPIO_PinState buttonState = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);

	switch (debounceState) {

	case BUTTON_UP:

		if (GPIO_PIN_RESET == buttonState) {	//if the button was pressed change the state to verify if it was noise or not
			debounceState = BUTTON_FALLING;
			delayRead(&debounceDelay);
		}

		break;

	case BUTTON_FALLING:

		if (delayRead(&debounceDelay)){				//if the amount of time that we have to wait has passed verify if the button
			if (GPIO_PIN_RESET == buttonState) {	//continue pressed or not, if so send the state to button pressed (BUTTON_DOWN), else
				debounceState = BUTTON_DOWN;		//go back to button up because it was noise.
				buttonPressed();
			}
			else {
				debounceState = BUTTON_UP;
			}
		}

		break;
	case BUTTON_DOWN:

		if (GPIO_PIN_SET == buttonState) {		//if the button was released change the state to verify if it was noise or not
			debounceState = BUTTON_RAISING;
			delayRead(&debounceDelay);
		}


		break;

	case BUTTON_RAISING:

		if (delayRead(&debounceDelay)){				//if the amount of time that we have to wait has passed, verify if the button
			if (GPIO_PIN_SET == buttonState) {		//continue released or not, if so send the state to button release (BUTTON_UP), else
				debounceState = BUTTON_UP;			//go back to button down because it was noise.
				buttonReleased();
			}
			else {
				debounceState = BUTTON_DOWN;
			}
		}

		break;

	default:
		debounceState = BUTTON_UP;
		buttonEdge = false;
		break;

	}

}


void buttonPressed() {
//	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  buttonEdge = true;
}



void buttonReleased(){
//	 HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

bool_t readKey(){
	if(buttonEdge){
		buttonEdge = false;
		return true;
	}
	else
		return false;
}




