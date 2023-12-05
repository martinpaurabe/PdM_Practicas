/*
 * API_LCD.h
 *
 *  Created on: Dec 3, 2023
 *      Author: martin
 */

#ifndef API_INC_API_LCD_H_
#define API_INC_API_LCD_H_


#include "API_delay.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "main.h"

void LCD_Init(void);
void LCD_SendCmd(uint8_t Cmd);
void LCD_SendChar(uint8_t Data);
void LCD_Clr(void);
void LCD_SendStr(char *str);



#endif /* API_INC_API_LCD_H_ */
