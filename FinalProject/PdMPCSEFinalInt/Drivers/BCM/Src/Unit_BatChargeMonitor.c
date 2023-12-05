#include "main.h" //Include just to toggle the LED

#include <stdio.h>
#include <string.h>

#include "Unit_BatChargeMonitor.h"
#include "API_debounce.h"
#include "API_LCD.h"
#include "Unit_ThreadComPort.h"

//==============================================================================================================
// Private Definitions
//==============================================================================================================

#define BCD_VARIABLE_UD_PER  	100  //Period update in milisec
#define LCD_HOR_DIM  			20
#define LCD_POS_VARSHOW 		0x80|0x06
#define LCD_POS_MOD1 			0x80|0x00
#define LCD_POS_MOD2 			0x80|0x0F
#define LCD_POS_MOD3 			0x80|0x40
#define LCD_POS_MOD4 			0x80|0x4F
#define LCD_POS_MOD5 			0x80|0x10|0x04
#define LCD_POS_MOD6 			0x80|0x20|0x03
#define LCD_POS_MOD7 			0x80|0x50|0x04
#define LCD_POS_MOD8 			0x80|0x60|0x03


//==============================================================================================================
// Private Functions
//==============================================================================================================

static void BcmFsmFuncShowCurrent(void);
static void BcmFsmFuncShowPWM(void);
static void BcmFsmFuncShowPorcent(void);
static void BcmFsmFuncShowComplet(void);
static void BatChargerStt_Update(void);
static bool_t BatPackChrgd(float limit);


//==============================================================================================================
// Private Variable Types Definitions
//==============================================================================================================

typedef struct
{
    float   Porc; /* data */
    float   PWM; /* data */
    float   Curr; /* data */
    float   Volt; /* data */
}TChargerMod;



typedef struct
{
    TChargerMod  ChargerMod1; /* data */
    TChargerMod  ChargerMod2; /* data */
    TChargerMod  ChargerMod3; /* data */
    TChargerMod  ChargerMod4; /* data */
    TChargerMod  ChargerMod5; /* data */
    TChargerMod  ChargerMod6; /* data */
    TChargerMod  ChargerMod7; /* data */
    TChargerMod  ChargerMod8; /* data */
    uint8_t BcmFsmStt;
    bool_t  BcmFsmSttChgd;
    uint8_t num_ModuloUD;
}TBatChargeMon;


//==============================================================================================================
// Private Variable
//==============================================================================================================

static const TBatChargeMon BatChargeMonFlash;
static TBatChargeMon BatChargeMon;
static delay_t DataUpdate;



enum {BCM_FSM_SHOWPORCENT,BCM_FSM_SHOWPWM,BCM_FSM_SHOWCURR,BCM_FSM_SHOWCOMPLET};

enum {MODULO0,MODULO1,MODULO2,MODULO3,MODULO4,MODULO5,MODULO6,MODULO7,MODULO8};

//==============================================================================================================
// BatChargMon_Init
//==============================================================================================================

void BatChargMon_Init(void)
{
  BatChargeMon=BatChargeMonFlash;
  BatChargeMon.BcmFsmStt  = BCM_FSM_SHOWPORCENT;
  BatChargeMon.BcmFsmSttChgd  = true;
  BatChargeMon.num_ModuloUD = MODULO1;
  debounceFSM_init();
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_SET);
  LCD_Init();
  ThreadComPort_Init();
  delayInit(&DataUpdate,BCD_VARIABLE_UD_PER);
  delayInit(&DataUpdate,BCD_VARIABLE_UD_PER);
}


//==============================================================================================================
// BatChargMon_Init
//==============================================================================================================

void BatChargMon_Update(void)
{
	debounceFSM_update();
//	ThreadComPort_Update();
	if(delayRead(&DataUpdate))
		BatChargerStt_Update();

	switch (BatChargeMon.BcmFsmStt)
	{
	case BCM_FSM_SHOWPORCENT:
	  if (BatChargeMon.BcmFsmSttChgd)
	  {
		  BatChargeMon.BcmFsmSttChgd =false;
		  LCD_Clr();
	  }
	  BcmFsmFuncShowPorcent();
	  if(readKeyPosEdge())
	  {
		  BatChargeMon.BcmFsmSttChgd = true;
		  BatChargeMon.BcmFsmStt = BCM_FSM_SHOWPWM;

	  }
	  if(BatPackChrgd(BAT_CHRGD))//if so, it mast switch the period of blinking and send a msg through UART
	  {
		  BatChargeMon.BcmFsmSttChgd = true;
		  BatChargeMon.BcmFsmStt = BCM_FSM_SHOWCOMPLET;

	  }
	  break;
	case BCM_FSM_SHOWPWM:
	  if (BatChargeMon.BcmFsmSttChgd)
	  {
		  BatChargeMon.BcmFsmSttChgd =false;
		  LCD_Clr();
	  }
	  BcmFsmFuncShowPWM();
	  if(readKeyPosEdge())
	  {
		  BatChargeMon.BcmFsmSttChgd = true;
		  BatChargeMon.BcmFsmStt = BCM_FSM_SHOWCURR;

	  }//if so, it mast switch the period of blinking and send a msg through UART
	  if(BatPackChrgd(BAT_CHRGD))//if so, it mast switch the period of blinking and send a msg through UART
	  {
		  BatChargeMon.BcmFsmSttChgd = true;
		  BatChargeMon.BcmFsmStt = BCM_FSM_SHOWCOMPLET;
	  }
	  break;
	case BCM_FSM_SHOWCURR:
	  if (BatChargeMon.BcmFsmSttChgd)
	  {
		  BatChargeMon.BcmFsmSttChgd =false;
		  LCD_Clr();
	  }
	  BcmFsmFuncShowCurrent();
	  if(readKeyPosEdge())
	  {
		  BatChargeMon.BcmFsmSttChgd = true;
		  BatChargeMon.BcmFsmStt = BCM_FSM_SHOWPORCENT;

	  }//if so, it mast switch the period of blinking and send a msg through UART
	  if(BatPackChrgd(BAT_CHRGD))//if so, it mast switch the period of blinking and send a msg through UART
	  {
		  BatChargeMon.BcmFsmSttChgd = true;
		  BatChargeMon.BcmFsmStt = BCM_FSM_SHOWCOMPLET;
	  }
	  break;
	case BCM_FSM_SHOWCOMPLET:
	  if (BatChargeMon.BcmFsmSttChgd)
	  {
		  BatChargeMon.BcmFsmSttChgd =false;
		  LCD_Clr();
	  }
	  BcmFsmFuncShowComplet();
	  if(!BatPackChrgd(BAT_DISCHRGD))//if so, it mast switch the period of blinking and send a msg through UART
	  {
		  BatChargeMon.BcmFsmSttChgd = true;
		  BatChargeMon.BcmFsmStt = BCM_FSM_SHOWPORCENT;
	  }
	  break;
	default:
		BatChargeMon.BcmFsmStt = BCM_FSM_SHOWPWM;
		BatChargeMon.BcmFsmSttChgd = true;
	  break;
	}
}

//==============================================================================================================
// BatChargMon_Init
//==============================================================================================================

void ThreadComPort_RxMsg(uint8_t Comand, uint8_t *Data, uint8_t DataLen)
{
  TDato32 *dato;
  uint8_t dataLen32=0;
  int i;


  switch(Comand)
  {
//Serial Data Request
  case DRQ_BCM_MOD1:
	ThreadComPort_SendMsg(DAT_BCM_MOD1, &BatChargeMon.ChargerMod1, sizeof(BatChargeMon.ChargerMod1));
  break;
  case DRQ_BCM_MOD2:
	ThreadComPort_SendMsg(DAT_BCM_MOD2, &BatChargeMon.ChargerMod2, sizeof(BatChargeMon.ChargerMod2));
  break;

  case CMD_BCM_MOD1:
	ThreadComPort_SendMsg(DAT_BCM_MOD1, &BatChargeMon.ChargerMod1, sizeof(BatChargeMon.ChargerMod1));
  break;

//Serial Data Recieve
  case DAT_BCM_MOD1:
    dato = (TDato32 *)&BatChargeMon.ChargerMod1;
    dataLen32 = sizeof(BatChargeMon.ChargerMod1)/4;
  break;
  case DAT_BCM_MOD2:
    dato = (TDato32 *)&BatChargeMon.ChargerMod2;
    dataLen32 = sizeof(BatChargeMon.ChargerMod2)/4;
  break;
  case DAT_BCM_MOD3:
    dato = (TDato32 *)&BatChargeMon.ChargerMod3;
    dataLen32 = sizeof(BatChargeMon.ChargerMod3)/4;
  break;
  case DAT_BCM_MOD4:
    dato = (TDato32 *)&BatChargeMon.ChargerMod4;
    dataLen32 = sizeof(BatChargeMon.ChargerMod4)/4;
  break;
  case DAT_BCM_MOD5:
    dato = (TDato32 *)&BatChargeMon.ChargerMod5;
    dataLen32 = sizeof(BatChargeMon.ChargerMod5)/4;
  break;
  case DAT_BCM_MOD6:
    dato = (TDato32 *)&BatChargeMon.ChargerMod6;
    dataLen32 = sizeof(BatChargeMon.ChargerMod6)/4;
  break;
  case DAT_BCM_MOD7:
    dato = (TDato32 *)&BatChargeMon.ChargerMod7;
    dataLen32 = sizeof(BatChargeMon.ChargerMod7)/4;
  break;
  case DAT_BCM_MOD8:
    dato = (TDato32 *)&BatChargeMon.ChargerMod8;
    dataLen32 = sizeof(BatChargeMon.ChargerMod8)/4;
  break;
  default:
  return;
  }

//If it have received new data, it have to reverse the bytes order
  for(i = 0; i < dataLen32; i++)
  {
    dato[i].byt[0] = Data[4*i+0];
    dato[i].byt[1] = Data[4*i+1];
    dato[i].byt[2] = Data[4*i+2];
    dato[i].byt[3] = Data[4*i+3];
  }

}
//--------------------------------------------------------------------------------------------------------------

static void BcmFsmFuncShowPWM(void)
{
char MSG_LCD[LCD_HOR_DIM];
int PWM;
LCD_SendCmd (LCD_POS_VARSHOW); //Writing on the first line
LCD_SendStr("   PWM");
PWM = (10.0*BatChargeMon.ChargerMod1.PWM);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= PWM%10 + '0';
PWM/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= PWM%10 + '0';
PWM/=10;
MSG_LCD[0]= PWM%10 + '0';
LCD_SendCmd (LCD_POS_MOD1); //Writing on LCD Module 1 position
LCD_SendStr(MSG_LCD);
PWM = (10.0*BatChargeMon.ChargerMod2.PWM);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= PWM%10 + '0';
PWM/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= PWM%10 + '0';
PWM/=10;
MSG_LCD[0]= PWM%10 + '0';
LCD_SendCmd (LCD_POS_MOD2); //Writing on LCD Module 3 position
LCD_SendStr(MSG_LCD);
PWM = (10.0*BatChargeMon.ChargerMod3.PWM);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= PWM%10 + '0';
PWM/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= PWM%10 + '0';
PWM/=10;
MSG_LCD[0]= PWM%10 + '0';
LCD_SendCmd (LCD_POS_MOD3); //Writing on LCD Module 3 position
LCD_SendStr(MSG_LCD);
PWM = (10.0*BatChargeMon.ChargerMod4.PWM);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= PWM%10 + '0';
PWM/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= PWM%10 + '0';
PWM/=10;
MSG_LCD[0]= PWM%10 + '0';
LCD_SendCmd (LCD_POS_MOD4);//Writing on LCD Module 4 position
LCD_SendStr(MSG_LCD);
PWM = (10.0*BatChargeMon.ChargerMod5.PWM);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= PWM%10 + '0';
PWM/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= PWM%10 + '0';
PWM/=10;
MSG_LCD[0]= PWM%10 + '0';
LCD_SendCmd (LCD_POS_MOD5); //Writing on LCD Module 5 position
LCD_SendStr(MSG_LCD);
PWM = (10.0*BatChargeMon.ChargerMod6.PWM);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= PWM%10 + '0';
PWM/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= PWM%10 + '0';
PWM/=10;
MSG_LCD[0]= PWM%10 + '0';
LCD_SendCmd (LCD_POS_MOD6); //Writing on LCD Module 6 position
LCD_SendStr(MSG_LCD);
PWM = (10.0*BatChargeMon.ChargerMod7.PWM);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= PWM%10 + '0';
PWM/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= PWM%10 + '0';
PWM/=10;
MSG_LCD[0]= PWM%10 + '0';
LCD_SendCmd (LCD_POS_MOD7);//Writing on LCD Module 7 position
LCD_SendStr(MSG_LCD);
PWM = (10.0*BatChargeMon.ChargerMod8.PWM);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= PWM%10 + '0';
PWM/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= PWM%10 + '0';
PWM/=10;
MSG_LCD[0]= PWM%10 + '0';
LCD_SendCmd (LCD_POS_MOD8);//Writing on LCD Module 8 position
LCD_SendStr(MSG_LCD);
}


//--------------------------------------------------------------------------------------------------------------
static void BcmFsmFuncShowPorcent(void)
{
char MSG_LCD[LCD_HOR_DIM];
int Porcent;
LCD_SendCmd (LCD_POS_VARSHOW); //Writing on the first line
LCD_SendStr(" PORCENT");
Porcent = (10.0*BatChargeMon.ChargerMod1.Porc);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= Porcent%10 + '0';
Porcent/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Porcent%10 + '0';
Porcent/=10;
MSG_LCD[0]= Porcent%10 + '0';
LCD_SendCmd (LCD_POS_MOD1); //Writing on LCD Module 1 position
LCD_SendStr(MSG_LCD);
Porcent = (10.0*BatChargeMon.ChargerMod2.Porc);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= Porcent%10 + '0';
Porcent/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Porcent%10 + '0';
Porcent/=10;
MSG_LCD[0]= Porcent%10 + '0';
LCD_SendCmd (LCD_POS_MOD2); //Writing on LCD Module 3 position
LCD_SendStr(MSG_LCD);
Porcent = (10.0*BatChargeMon.ChargerMod3.Porc);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= Porcent%10 + '0';
Porcent/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Porcent%10 + '0';
Porcent/=10;
MSG_LCD[0]= Porcent%10 + '0';
LCD_SendCmd (LCD_POS_MOD3); //Writing on LCD Module 3 position
LCD_SendStr(MSG_LCD);
Porcent = (10.0*BatChargeMon.ChargerMod4.Porc);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= Porcent%10 + '0';
Porcent/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Porcent%10 + '0';
Porcent/=10;
MSG_LCD[0]= Porcent%10 + '0';
LCD_SendCmd (LCD_POS_MOD4);//Writing on LCD Module 4 position
LCD_SendStr(MSG_LCD);
Porcent = (10.0*BatChargeMon.ChargerMod5.Porc);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= Porcent%10 + '0';
Porcent/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Porcent%10 + '0';
Porcent/=10;
MSG_LCD[0]= Porcent%10 + '0';
LCD_SendCmd (LCD_POS_MOD5); //Writing on LCD Module 5 position
LCD_SendStr(MSG_LCD);
Porcent = (10.0*BatChargeMon.ChargerMod6.Porc);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= Porcent%10 + '0';
Porcent/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Porcent%10 + '0';
Porcent/=10;
MSG_LCD[0]= Porcent%10 + '0';
LCD_SendCmd (LCD_POS_MOD6); //Writing on LCD Module 6 position
LCD_SendStr(MSG_LCD);
Porcent = (10.0*BatChargeMon.ChargerMod7.Porc);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= Porcent%10 + '0';
Porcent/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Porcent%10 + '0';
Porcent/=10;
MSG_LCD[0]= Porcent%10 + '0';
LCD_SendCmd (LCD_POS_MOD7);//Writing on LCD Module 7 position
LCD_SendStr(MSG_LCD);
Porcent = (10.0*BatChargeMon.ChargerMod8.Porc);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= Porcent%10 + '0';
Porcent/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Porcent%10 + '0';
Porcent/=10;
MSG_LCD[0]= Porcent%10 + '0';
LCD_SendCmd (LCD_POS_MOD8);//Writing on LCD Module 8 position
LCD_SendStr(MSG_LCD);
}

static void BcmFsmFuncShowCurrent(void)
{
char MSG_LCD[LCD_HOR_DIM];
int Current;
LCD_SendCmd (LCD_POS_VARSHOW); //Writing on the first line
LCD_SendStr(" CURRENT  ");
Current = (10.0*BatChargeMon.ChargerMod1.Curr);
MSG_LCD[5]=0;
MSG_LCD[4]='A';
MSG_LCD[3]=Current%10 + '0';
Current/=10;
MSG_LCD[2]='.';
MSG_LCD[1]=Current%10 + '0';
Current/=10;
MSG_LCD[0]=Current%10 + '0';
LCD_SendCmd (LCD_POS_MOD1); //Writing on LCD Module 1 position
LCD_SendStr(MSG_LCD);
Current = (10.0*BatChargeMon.ChargerMod2.Curr);
MSG_LCD[5]=0;
MSG_LCD[4]='A';
MSG_LCD[3]=Current%10 + '0';
Current/=10;
MSG_LCD[2]='.';
MSG_LCD[1]=Current%10 + '0';
Current/=10;
MSG_LCD[0]=Current%10 + '0';
LCD_SendCmd (LCD_POS_MOD2); //Writing on LCD Module 3 position
LCD_SendStr(MSG_LCD);
Current = (10.0*BatChargeMon.ChargerMod3.Curr);
MSG_LCD[5]=0;
MSG_LCD[4]='A';
MSG_LCD[3]=Current%10 + '0';
Current/=10;
MSG_LCD[2]='.';
MSG_LCD[1]=Current%10 + '0';
Current/=10;
MSG_LCD[0]=Current%10 + '0';
LCD_SendCmd (LCD_POS_MOD3); //Writing on LCD Module 3 position
LCD_SendStr(MSG_LCD);
Current = (10.0*BatChargeMon.ChargerMod4.Curr);
MSG_LCD[5]=0;
MSG_LCD[4]='A';
MSG_LCD[3]=Current%10 + '0';
Current/=10;
MSG_LCD[2]='.';
MSG_LCD[1]=Current%10 + '0';
Current/=10;
MSG_LCD[0]=Current%10 + '0';
LCD_SendCmd (LCD_POS_MOD4);//Writing on LCD Module 4 position
LCD_SendStr(MSG_LCD);
Current = (10.0*BatChargeMon.ChargerMod5.Curr);
MSG_LCD[5]=0;
MSG_LCD[4]='A';
MSG_LCD[3]=Current%10 + '0';
Current/=10;
MSG_LCD[2]='.';
MSG_LCD[1]=Current%10 + '0';
Current/=10;
MSG_LCD[0]=Current%10 + '0';
LCD_SendCmd (LCD_POS_MOD5); //Writing on LCD Module 5 position
LCD_SendStr(MSG_LCD);
Current = (10.0*BatChargeMon.ChargerMod6.Curr);
MSG_LCD[5]=0;
MSG_LCD[4]='A';
MSG_LCD[3]=Current%10 + '0';
Current/=10;
MSG_LCD[2]='.';
MSG_LCD[1]=Current%10 + '0';
Current/=10;
MSG_LCD[0]=Current%10 + '0';
LCD_SendCmd (LCD_POS_MOD6); //Writing on LCD Module 6 position
LCD_SendStr(MSG_LCD);
Current = (10.0*BatChargeMon.ChargerMod7.Curr);
MSG_LCD[5]=0;
MSG_LCD[4]='A';
MSG_LCD[3]=Current%10 + '0';
Current/=10;
MSG_LCD[2]='.';
MSG_LCD[1]=Current%10 + '0';
Current/=10;
MSG_LCD[0]=Current%10 + '0';
LCD_SendCmd (LCD_POS_MOD7);//Writing on LCD Module 7 position
LCD_SendStr(MSG_LCD);
Current = (10.0*BatChargeMon.ChargerMod8.Curr);
MSG_LCD[5]=0;
MSG_LCD[4]='A';
MSG_LCD[3]=Current%10 + '0';
Current/=10;
MSG_LCD[2]='.';
MSG_LCD[1]=Current%10 + '0';
Current/=10;
MSG_LCD[0]=Current%10 + '0';
LCD_SendCmd (LCD_POS_MOD8);//Writing on LCD Module 8 position
LCD_SendStr(MSG_LCD);
}

//--------------------------------------------------------------------------------------------------------------
void BcmFsmFuncShowComplet(void)
{
LCD_SendCmd (0x80|0x00); //Writing on the first line
LCD_SendStr("                     ");
LCD_SendCmd (0x80|0x40); //Writing on the Second line
LCD_SendStr("        CARGA        ");
LCD_SendCmd (0x80|0x10|0x04); //Writing on the first line
LCD_SendStr("       COMPLETA      ");
LCD_SendCmd (0x80|0x50|0x04); //Writing on the first line
LCD_SendStr("                     ");
}

bool_t BatPackChrgd(float limit)
{
	bool_t Charged = false;
	if(BatChargeMon.ChargerMod1.Porc>limit &&
			BatChargeMon.ChargerMod2.Porc>limit &&
			BatChargeMon.ChargerMod3.Porc>limit &&
			BatChargeMon.ChargerMod4.Porc>limit &&
			BatChargeMon.ChargerMod5.Porc>limit &&
			BatChargeMon.ChargerMod6.Porc>limit &&
			BatChargeMon.ChargerMod7.Porc>limit &&
			BatChargeMon.ChargerMod8.Porc>limit)
		Charged = true;
	return Charged;
}



void BatChargerStt_Update(void)
{
  switch (BatChargeMon.num_ModuloUD)
  {
  case MODULO1:
	ThreadComPort_SendMsg(DRQ_BCM_MOD1, &BatChargeMon.ChargerMod1, sizeof(BatChargeMon.ChargerMod1));
	BatChargeMon.num_ModuloUD = MODULO2;
  break;
  case MODULO2:
	ThreadComPort_SendMsg(DRQ_BCM_MOD2, &BatChargeMon.ChargerMod2, sizeof(BatChargeMon.ChargerMod2));
	BatChargeMon.num_ModuloUD = MODULO3;
  break;
  case MODULO3:
	ThreadComPort_SendMsg(DRQ_BCM_MOD3, &BatChargeMon.ChargerMod3, sizeof(BatChargeMon.ChargerMod3));
	BatChargeMon.num_ModuloUD = MODULO4;
  break;
  case MODULO4:
	ThreadComPort_SendMsg(DRQ_BCM_MOD4, &BatChargeMon.ChargerMod4, sizeof(BatChargeMon.ChargerMod4));
	BatChargeMon.num_ModuloUD = MODULO5;
 break;
  case MODULO5:
	ThreadComPort_SendMsg(DRQ_BCM_MOD5, &BatChargeMon.ChargerMod5, sizeof(BatChargeMon.ChargerMod5));
	BatChargeMon.num_ModuloUD = MODULO6;
  break;
  case MODULO6:
	ThreadComPort_SendMsg(DRQ_BCM_MOD6, &BatChargeMon.ChargerMod6, sizeof(BatChargeMon.ChargerMod6));
	BatChargeMon.num_ModuloUD = MODULO7;
  break;
  case MODULO7:
	ThreadComPort_SendMsg(DRQ_BCM_MOD7, &BatChargeMon.ChargerMod7, sizeof(BatChargeMon.ChargerMod7));
	BatChargeMon.num_ModuloUD = MODULO8;
  break;
  case MODULO8:
	ThreadComPort_SendMsg(DRQ_BCM_MOD8, &BatChargeMon.ChargerMod8, sizeof(BatChargeMon.ChargerMod8));
	BatChargeMon.num_ModuloUD = MODULO1;
  break;
  default:
	BatChargeMon.num_ModuloUD = MODULO1;
  break;
  }

}

