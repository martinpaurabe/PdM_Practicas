/*
 * API_LCD.c
 *
 *  Created on: Dec 3, 2023
 *      Author: martin
 */
#include "API_LCD.h"
#include "API_delay.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"
//#include "main.h"

//Private Definitions
//LCD Initialize
#define INITIALIZATION_FIRST_WAIT   20
#define INITIALIZATION_FIRST_BYT    0x03
#define INITIALIZATION_SECOND_WAIT  10
#define INITIALIZATION_SECOND_BYT   0x03
#define INITIALIZATION_THIRD_WAIT 	1
#define INITIALIZATION_THIRD_BYT  	0x03
#define INITIALIZATION_FOURTH_WAIT 	1
#define INITIALIZATION_FOURTH_BYT  	0x02
#define INITIALIZATION_FIFTH_WAIT  	100
#define LCD_POS_INIT_MSG            0x80|0x05
#define INITIALIZATION_MSG_TIME  	2000


#define LCD_ADDRESS	  0x4E

#define _4BIT_MODE	  0x28
#define DISPLAY_CTRL  (1<<3)
#define RETURN_HOME   (1<<1)
#define ENTRY_MODE 	  (1<<2)
#define AUTOINCREMENT (1<<1)
#define DISPLAY_CTRL  (1<<3)
#define DISPLAY_ON    (1<<2)
#define CLR_LCD		  (1<<0)


static uint8_t LCD_INT_CMD[]={INITIALIZATION_FIRST_BYT,INITIALIZATION_SECOND_BYT,
		INITIALIZATION_THIRD_BYT,INITIALIZATION_FOURTH_BYT,_4BIT_MODE,DISPLAY_CTRL,
		RETURN_HOME,ENTRY_MODE|AUTOINCREMENT,DISPLAY_CTRL|DISPLAY_ON,CLR_LCD};

//Private Variables
static I2C_HandleTypeDef hi2c1;
static delay_t LCD_InitDelay;


//Private Functions
static void MX_I2C1_Init(void);
static void Error_Handler(void);


void LCD_Init(void)
{
	MX_I2C1_Init();

	uint8_t *Dato = LCD_INT_CMD;

	delayInit(&LCD_InitDelay,INITIALIZATION_FIRST_WAIT);
	while(!delayRead(&LCD_InitDelay));
	LCD_SendCmd(*Dato++);
	delayWrite(&LCD_InitDelay,INITIALIZATION_SECOND_WAIT);
	while(!delayRead(&LCD_InitDelay));
	LCD_SendCmd(*Dato++);
	delayWrite(&LCD_InitDelay,INITIALIZATION_THIRD_WAIT);
	while(!delayRead(&LCD_InitDelay));
	LCD_SendCmd(*Dato++);
	delayWrite(&LCD_InitDelay,INITIALIZATION_FOURTH_WAIT);
	while(!delayRead(&LCD_InitDelay));
	LCD_SendCmd(*Dato++);
	delayWrite(&LCD_InitDelay,INITIALIZATION_FIFTH_WAIT);
	while(!delayRead(&LCD_InitDelay));
	LCD_SendCmd(*Dato++);
	LCD_SendCmd(*Dato++);
	LCD_SendCmd(*Dato++);
	LCD_SendCmd(*Dato++);
	LCD_SendCmd(*Dato++);
	LCD_SendCmd(*Dato++);


	LCD_SendCmd(LCD_POS_LINE1); //Writing on the first line
    LCD_SendStr("                    ");
	LCD_SendCmd(LCD_POS_LINE2); //Writing on the SECOND line
    LCD_SendStr("       DEVICE       ");
	LCD_SendCmd(LCD_POS_LINE3); //Writing on the THIRD line
    LCD_SendStr("        INIT        ");
	LCD_SendCmd(LCD_POS_LINE4); //Writing on the FOURTH line
    LCD_SendStr("                    ");
    HAL_Delay(INITIALIZATION_MSG_TIME);
}



void LCD_SendCmd(uint8_t Cmd)
{
	uint8_t data_u, data_l;
	uint8_t data_t[4];
	data_u = (Cmd&0xf0);
	data_l = ((Cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //send High and low through enable pin
	data_t[1] = data_u|0x08;  //twice to send upper and lower nible
	data_t[2] = data_l|0x0C;  //RS in zero to send command
	data_t[3] = data_l|0x08;
	HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS,(uint8_t *) data_t, 4, 100);
}

void LCD_SendChar(uint8_t Data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (Data&0xf0);
	data_l = ((Data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //send High and low through enable pin
	data_t[1] = data_u|0x09;  //twice to send upper and lower nibble
	data_t[2] = data_l|0x0D;  //RS in one to send DAta
	data_t[3] = data_l|0x09;
	HAL_I2C_Master_Transmit (&hi2c1, LCD_ADDRESS,(uint8_t *) data_t, 4, 100);
}


void LCD_Clr(void)
{
	LCD_SendCmd(CLR_LCD);
	for (int i=0; i<100; i++)
	{
		LCD_SendChar(' ');
	}
}


void LCD_SendStr(char *str)
{
	while (*str) LCD_SendChar(*str++);
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}


static void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
