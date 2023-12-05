/*
 * API_LCD.h
 *
 *  Created on: Dec 3, 2023
 *      Author: martin
 */

#ifndef API_INC_API_LCD_H_
#define API_INC_API_LCD_H_

#include "Def_TipoDatos.h"

#define LCD_POS_LINE1 	0x80|0x00
#define LCD_POS_LINE2 	0x80|0x40
#define LCD_POS_LINE3 	0x80|0x10|0x04
#define LCD_POS_LINE4	0x80|0x50|0x04




void LCD_Init(void);
void LCD_SendCmd(uint8_t Cmd);
void LCD_SendChar(uint8_t Data);
void LCD_Clr(void);
void LCD_SendStr(char *str);

#endif /* API_INC_API_LCD_H_ */
