#include "main.h"

#include <stdio.h>
#include <string.h>

#include "Unit_BatChargeMonitor.h"


static void BcmFsmFuncShowCurrent(void);
static void BcmFsmFuncShowPorcent(void);
static void BcmFsmFuncShowComplet(void);
static void BatChargerStt_Update(void);

//--------------------------------------------------------------------------------------------------------------


static TBatChargeMon BatChargeMon;
static delay_t DataUpdate;


static const TBatChargeMon BatChargeMonFlash;

static enum {BCM_FSM_SHOWPORCENT,BCM_FSM_SHOWCURRENT,BCM_FSM_SHOWCOMPLET};
static uint8_t BcmFsmStt = BCM_FSM_SHOWPORCENT;
static bool_t  BcmFsmSttChgd = true;

enum {MODULO0,MODULO1,MODULO2,MODULO3,MODULO4,MODULO5,MODULO6,MODULO7,MODULO8};

static uint8_t num_ModuloUD = MODULO1;


void BatChargMon_Init(void)
{
  BatChargeMon=BatChargeMonFlash;
  debounceFSM_init();
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_SET);
  LCD_Init();
  ThreadComPort_Init();
  delayInit(&DataUpdate,10);
  BcmFsmStt = BCM_FSM_SHOWPORCENT;

 
}

void BatChargMon_Update(void)
{
	debounceFSM_update();
//	ThreadComPort_Update();
	if(delayRead(&DataUpdate))
		BatChargerStt_Update();

	switch (BcmFsmStt)
	{
	case BCM_FSM_SHOWPORCENT:
	  if (BcmFsmSttChgd)
	  {
		  BcmFsmSttChgd =false;
		  LCD_Clr();
	  }
	  BcmFsmFuncShowPorcent();
	  if(readKeyPosEdge())
	  {
		  BcmFsmSttChgd = true;
		  BcmFsmStt = BCM_FSM_SHOWCURRENT;

	  }
	  if(BatPackChrgd(70))//if so, it mast switch the period of blinking and send a msg through UART
	  {
		  BcmFsmSttChgd = true;
		  BcmFsmStt = BCM_FSM_SHOWCOMPLET;

	  }
	  break;
	case BCM_FSM_SHOWCURRENT:
	  if (BcmFsmSttChgd)
	  {
		  BcmFsmSttChgd =false;
		  LCD_Clr();
	  }
	  BcmFsmFuncShowCurrent();
	  if(readKeyPosEdge())
	  {
		  BcmFsmSttChgd = true;
		  BcmFsmStt = BCM_FSM_SHOWPORCENT;

	  }//if so, it mast switch the period of blinking and send a msg through UART
	  break;
	case BCM_FSM_SHOWCOMPLET:
	  if (BcmFsmSttChgd)
	  {
		  BcmFsmSttChgd =false;
		  LCD_Clr();
	  }
	  BcmFsmFuncShowComplet();
	  if(!BatPackChrgd(50))//if so, it mast switch the period of blinking and send a msg through UART
	  {
		  BcmFsmSttChgd = true;
		  BcmFsmStt = BCM_FSM_SHOWPORCENT;
	  }
	  break;
	default:
	  BcmFsmStt = BCM_FSM_SHOWCURRENT;
	  break;
	}
}

//--------------------------------------------------------------------------------------------------------------

void sciDataReceived(BYTE *buf)
{
  TDato32 *dato;
  BYTE dataLen32=0;
  int i;


  switch(buf[0])
  {
//Serial Data Request
  case DRQ_BCM_MOD1:
	ThreadComPort_SendMsg(DAT_BCM_MOD1, &BatChargeMon.ChargerMod1, sizeof(BatChargeMon.ChargerMod1));
  break;
  case DRQ_BCM_MOD2:
	ThreadComPort_SendMsg(DAT_BCM_MOD2, &BatChargeMon.ChargerMod2, sizeof(BatChargeMon.ChargerMod2));
  break;
//Serial Command Recieve
/*
  case CMD_GES_STT_AEG_OFF:
    HabAeroGenOff;
    LedAeroGenOff;
  	GestEnergia.Gral.Status.dwrd &=~ STAT_GES_AEG;
    sendSciMsg(DRQ_GES_GRAL, &GestEnergia.Gral, sizeof(GestEnergia.Gral));
  break;
  case CMD_GES_STT_SOL_ON:
  	HabPanSolarOn;
  	LedPanSolarOn;
  	GestEnergia.Gral.Status.dwrd|=STAT_GES_SOL;
    sendSciMsg(DRQ_GES_GRAL, &GestEnergia.Gral, sizeof(GestEnergia.Gral));
  break;
*/
//Seria Data Recieve
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

//If it have recieve new data, it have to reverse de bytes order
  for(i = 0; i < dataLen32; i++)
  {
    dato[i].byt[0] = buf[4*i+1];
    dato[i].byt[1] = buf[4*i+2];
    dato[i].byt[2] = buf[4*i+3];
    dato[i].byt[3] = buf[4*i+4];
  }

}
//--------------------------------------------------------------------------------------------------------------
void BcmFsmFuncShowCurrent(void)
{
char MSG_LCD[20];
int Current;
LCD_SendCmd (0x80|0x06); //Writing on the first line
LCD_SendStr("CURRENT");
Current = (10.0*BatChargeMon.ChargerMod1.Curr);
MSG_LCD[5]=0;
MSG_LCD[4]='A';
MSG_LCD[3]= Current%10 + '0';
Current/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Current%10 + '0';
Current/=10;
MSG_LCD[0]= Current%10 + '0';
LCD_SendCmd (0x80|0x00); //Writing on the first line
LCD_SendStr(MSG_LCD);
Current = (10.0*BatChargeMon.ChargerMod2.Curr);
MSG_LCD[5]=0;
MSG_LCD[4]='A';
MSG_LCD[3]= Current%10 + '0';
Current/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Current%10 + '0';
Current/=10;
MSG_LCD[0]= Current%10 + '0';
LCD_SendCmd (0x80|0x0F); //Writing on the first line
LCD_SendStr(MSG_LCD);
Current = (10.0*BatChargeMon.ChargerMod3.Curr);
MSG_LCD[5]=0;
MSG_LCD[4]='A';
MSG_LCD[3]= Current%10 + '0';
Current/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Current%10 + '0';
Current/=10;
MSG_LCD[0]= Current%10 + '0';
LCD_SendCmd (0x80|0x40); //Writing on the first line
LCD_SendStr(MSG_LCD);
Current = (10.0*BatChargeMon.ChargerMod4.Curr);
MSG_LCD[5]=0;
MSG_LCD[4]='A';
MSG_LCD[3]= Current%10 + '0';
Current/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Current%10 + '0';
Current/=10;
MSG_LCD[0]= Current%10 + '0';
LCD_SendCmd (0x80|0x4F); //Writing on the first line
LCD_SendStr(MSG_LCD);
Current = (10.0*BatChargeMon.ChargerMod5.Curr);
MSG_LCD[5]=0;
MSG_LCD[4]='A';
MSG_LCD[3]= Current%10 + '0';
Current/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Current%10 + '0';
Current/=10;
MSG_LCD[0]= Current%10 + '0';
LCD_SendCmd (0x80|0x10|0x04); //Writing on the first line
LCD_SendStr(MSG_LCD);
Current = (10.0*BatChargeMon.ChargerMod6.Curr);
MSG_LCD[5]=0;
MSG_LCD[4]='A';
MSG_LCD[3]= Current%10 + '0';
Current/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Current%10 + '0';
Current/=10;
MSG_LCD[0]= Current%10 + '0';
LCD_SendCmd (0x80|0x20|0x03); //Writing on the first line
LCD_SendStr(MSG_LCD);
Current = (10.0*BatChargeMon.ChargerMod7.Curr);
MSG_LCD[5]=0;
MSG_LCD[4]='A';
MSG_LCD[3]= Current%10 + '0';
Current/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Current%10 + '0';
Current/=10;
MSG_LCD[0]= Current%10 + '0';
LCD_SendCmd (0x80|0x50|0x04);//Writing on the fourth line
LCD_SendStr(MSG_LCD);
Current = (10.0*BatChargeMon.ChargerMod8.Curr);
MSG_LCD[5]=0;
MSG_LCD[4]='A';
MSG_LCD[3]= Current%10 + '0';
Current/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Current%10 + '0';
Current/=10;
MSG_LCD[0]= Current%10 + '0';
LCD_SendCmd (0x80|0x60|0x03);//Writing on the fourth line
LCD_SendStr(MSG_LCD);
}


//--------------------------------------------------------------------------------------------------------------
void BcmFsmFuncShowPorcent(void)
{
char MSG_LCD[20];
int Porcentaje;
LCD_SendCmd (0x80|0x06); //Writing on the first line
LCD_SendStr("PORCENT");
Porcentaje = (10.0*BatChargeMon.ChargerMod1.Porc);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= Porcentaje%10 + '0';
Porcentaje/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Porcentaje%10 + '0';
Porcentaje/=10;
MSG_LCD[0]= Porcentaje%10 + '0';
LCD_SendCmd (0x80|0x00); //Writing on the first line
LCD_SendStr(MSG_LCD);
Porcentaje = (10.0*BatChargeMon.ChargerMod2.Porc);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= Porcentaje%10 + '0';
Porcentaje/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Porcentaje%10 + '0';
Porcentaje/=10;
MSG_LCD[0]= Porcentaje%10 + '0';
LCD_SendCmd (0x80|0x0F); //Writing on the first line
LCD_SendStr(MSG_LCD);
Porcentaje = (10.0*BatChargeMon.ChargerMod3.Porc);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= Porcentaje%10 + '0';
Porcentaje/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Porcentaje%10 + '0';
Porcentaje/=10;
MSG_LCD[0]= Porcentaje%10 + '0';
LCD_SendCmd (0x80|0x40); //Writing on the first line
LCD_SendStr(MSG_LCD);
Porcentaje = (10.0*BatChargeMon.ChargerMod4.Porc);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= Porcentaje%10 + '0';
Porcentaje/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Porcentaje%10 + '0';
Porcentaje/=10;
MSG_LCD[0]= Porcentaje%10 + '0';
LCD_SendCmd (0x80|0x4F); //Writing on the first line
LCD_SendStr(MSG_LCD);
Porcentaje = (10.0*BatChargeMon.ChargerMod5.Porc);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= Porcentaje%10 + '0';
Porcentaje/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Porcentaje%10 + '0';
Porcentaje/=10;
MSG_LCD[0]= Porcentaje%10 + '0';
LCD_SendCmd (0x80|0x10|0x04); //Writing on the first line
LCD_SendStr(MSG_LCD);
Porcentaje = (10.0*BatChargeMon.ChargerMod6.Porc);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= Porcentaje%10 + '0';
Porcentaje/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Porcentaje%10 + '0';
Porcentaje/=10;
MSG_LCD[0]= Porcentaje%10 + '0';
LCD_SendCmd (0x80|0x20|0x03); //Writing on the first line
LCD_SendStr(MSG_LCD);
Porcentaje = (10.0*BatChargeMon.ChargerMod7.Porc);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= Porcentaje%10 + '0';
Porcentaje/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Porcentaje%10 + '0';
Porcentaje/=10;
MSG_LCD[0]= Porcentaje%10 + '0';
LCD_SendCmd (0x80|0x50|0x04);//Writing on the fourth line
LCD_SendStr(MSG_LCD);
Porcentaje = (10.0*BatChargeMon.ChargerMod8.Porc);
MSG_LCD[5]=0;
MSG_LCD[4]='%';
MSG_LCD[3]= Porcentaje%10 + '0';
Porcentaje/=10;
MSG_LCD[2]='.';
MSG_LCD[1]= Porcentaje%10 + '0';
Porcentaje/=10;
MSG_LCD[0]= Porcentaje%10 + '0';
LCD_SendCmd (0x80|0x60|0x03);//Writing on the fourth line
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
  switch (num_ModuloUD)
  {
  case MODULO1:
	ThreadComPort_SendMsg(DRQ_BCM_MOD1, &BatChargeMon.ChargerMod1, sizeof(BatChargeMon.ChargerMod1));
    num_ModuloUD = MODULO2;
  break;
  case MODULO2:
	ThreadComPort_SendMsg(DRQ_BCM_MOD2, &BatChargeMon.ChargerMod2, sizeof(BatChargeMon.ChargerMod2));
    num_ModuloUD = MODULO3;
  break;
  case MODULO3:
	ThreadComPort_SendMsg(DRQ_BCM_MOD3, &BatChargeMon.ChargerMod3, sizeof(BatChargeMon.ChargerMod3));
    num_ModuloUD = MODULO4;
  break;
  case MODULO4:
	ThreadComPort_SendMsg(DRQ_BCM_MOD4, &BatChargeMon.ChargerMod4, sizeof(BatChargeMon.ChargerMod4));
    num_ModuloUD = MODULO5;
 break;
  case MODULO5:
	ThreadComPort_SendMsg(DRQ_BCM_MOD5, &BatChargeMon.ChargerMod5, sizeof(BatChargeMon.ChargerMod5));
    num_ModuloUD = MODULO6;
  break;
  case MODULO6:
	ThreadComPort_SendMsg(DRQ_BCM_MOD6, &BatChargeMon.ChargerMod6, sizeof(BatChargeMon.ChargerMod6));
    num_ModuloUD = MODULO7;
  break;
  case MODULO7:
	ThreadComPort_SendMsg(DRQ_BCM_MOD7, &BatChargeMon.ChargerMod7, sizeof(BatChargeMon.ChargerMod7));
    num_ModuloUD = MODULO8;
  break;
  case MODULO8:
	ThreadComPort_SendMsg(DRQ_BCM_MOD8, &BatChargeMon.ChargerMod8, sizeof(BatChargeMon.ChargerMod8));
    num_ModuloUD = MODULO1;
  break;
  default:
	num_ModuloUD = MODULO1;
  break;
  }

}

