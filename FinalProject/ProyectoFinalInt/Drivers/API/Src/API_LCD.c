/*
 * API_LCD.c
 *
 *  Created on: Dec 3, 2023
 *      Author: martin
 */
#include "API_LCD.h"


#define INITIALIZATION_FIRST_WAIT   20
#define INITIALIZATION_FIRST_BYT    0x03
#define INITIALIZATION_SECOND_WAIT  10
#define INITIALIZATION_SECOND_BYT   0x03
#define INITIALIZATION_THIRD_WAIT 	1
#define INITIALIZATION_THIRD_BYT  	0x03
#define INITIALIZATION_FOURTH_WAIT 	1
#define INITIALIZATION_FOURTH_BYT  	0x02

I2C_HandleTypeDef hi2c1;

delay_t LCD_InitDelay;

#define LCD_ADDRESS	0x4E

#define _4BIT_MODE	0x28
#define DISPLAY_CTRL (1<<3)
#define RETURN_HOME  (1<<1)
#define ENTRY_MODE 	 (1<<2)
#define AUTOINCREMENT (1<<1)
#define DISPLAY_CTRL (1<<3)
#define DISPLAY_ON   (1<<2)
#define CLR_LCD		(1<<0)


static const uint8_t LCD_INT_CMD[]={INITIALIZATION_FIRST_BYT,INITIALIZATION_SECOND_BYT,
		INITIALIZATION_THIRD_BYT,INITIALIZATION_FOURTH_BYT,_4BIT_MODE,DISPLAY_CTRL,
		RETURN_HOME,ENTRY_MODE|AUTOINCREMENT,DISPLAY_CTRL|DISPLAY_ON,CLR_LCD};

void LCD_SendCmd(uint8_t Cmd);
void LCD_SendChar(uint8_t Data);
void LCD_SendStr(char *str);
static void MX_I2C1_Init(void);

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
	delayWrite(&LCD_InitDelay,100);
	while(!delayRead(&LCD_InitDelay));
	LCD_SendCmd(*Dato++);
	LCD_SendCmd(*Dato++);
	LCD_SendCmd(*Dato++);
	LCD_SendCmd(*Dato++);
	LCD_SendCmd(*Dato++);
	LCD_SendCmd(*Dato++);


	LCD_SendCmd(0x80|0x05); //Writing on the first line
    LCD_SendStr("      INICIANDO     ");
    HAL_Delay(4000);

}



void LCD_SendCmd(uint8_t Cmd)
{
	uint8_t data_u, data_l;
	uint8_t data_t[4];
	data_u = (Cmd&0xf0);
	data_l = ((Cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, LCD_ADDRESS,(uint8_t *) data_t, 4, 100);
}

void LCD_SendChar(uint8_t Data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (Data&0xf0);
	data_l = ((Data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=1
	data_t[1] = data_u|0x09;  //en=0, rs=1
	data_t[2] = data_l|0x0D;  //en=1, rs=1
	data_t[3] = data_l|0x09;  //en=0, rs=1
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

/*
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
/*  hi2c1.Instance = I2C1;
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

//}

