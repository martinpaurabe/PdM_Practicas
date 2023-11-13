/*
 * API_debounce.c
 *
 *  Created on: Nov 9, 2023
 *      Author: Martin Paura
 */

#include "API_delay.h"
#include "API_debounce.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "main.h"


#define DEBOUNCETIME 40;

void buttonPressed();			// debe encender el LED
void buttonReleased();		// debe apagar el LED

typedef enum{
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RAISING,
} debounceState_t;

static debounceState_t debounceState;
static bool_t buttonEdge;
static delay_t debounceDelay;


void debounceFSM_init() {
	debounceState = BUTTON_UP;
	buttonEdge = false;
	tick_t initialDelay = DEBOUNCETIME;   // Set the initial time of the delay (200ms)
	delayInit(&debounceDelay, initialDelay); 	// Initialize the counter

}




void debounceFSM_update(){

	GPIO_PinState buttonState = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);

	switch (debounceState) {

	case BUTTON_UP:

		if (GPIO_PIN_RESET == buttonState) {
			debounceState = BUTTON_FALLING;
			delayRead(&debounceDelay);
		}

		break;

	case BUTTON_FALLING:

		if (delayRead(&debounceDelay)){
			if (GPIO_PIN_RESET == buttonState) {
				debounceState = BUTTON_DOWN;
				buttonPressed();
			}
			else {
				debounceState = BUTTON_UP;
			}
		}

		break;
	case BUTTON_DOWN:

		if (GPIO_PIN_SET == buttonState) {
			debounceState = BUTTON_RAISING;
			delayRead(&debounceDelay);
		}


		break;

	case BUTTON_RAISING:

		if (delayRead(&debounceDelay)){
			if (GPIO_PIN_SET == buttonState) {
				debounceState = BUTTON_UP;
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
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	  buttonEdge = true;
}



void buttonReleased(){
	 HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

bool_t readKey(){
	if(buttonEdge){
		buttonEdge = false;
		return true;
	}
	else
		return false;
}




