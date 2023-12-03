#include "main.h"

#include <stdio.h>
#include <string.h>

#include "Unit_Disp.h"



//--------------------------------------------------------------------------------------------------------------


TBatChargeMon BatChargeMon;


//const TBatChargeMon BatChargeMonFlash;


void BatChargerMonSim_Init(void)
{
//  BatChargeMon=BatChargeMonFlash;
BatChargeMon.ChargerMod1.Porc =10;
BatChargeMon.ChargerMod2.Porc =20;
BatChargeMon.ChargerMod3.Porc =30;
BatChargeMon.ChargerMod4.Porc =40;
BatChargeMon.ChargerMod5.Porc =50;
BatChargeMon.ChargerMod6.Porc =60;
BatChargeMon.ChargerMod7.Porc =70;
BatChargeMon.ChargerMod8.Porc =80;
return;
 
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
   printf("Llegó DRQ_BCM_MOD1\r\n");
   sendSciMsg(DAT_BCM_MOD1, &BatChargeMon.ChargerMod1, sizeof(BatChargeMon.ChargerMod1));
  break;
  case DRQ_BCM_MOD2:
    printf("Llegó DRQ_BCM_MOD2\r\n");
    sendSciMsg(DAT_BCM_MOD2, &BatChargeMon.ChargerMod2, sizeof(BatChargeMon.ChargerMod2));
  break;
  case DRQ_BCM_MOD3:
    printf("Llegó DRQ_BCM_MOD3\r\n");
    sendSciMsg(DAT_BCM_MOD3, &BatChargeMon.ChargerMod3, sizeof(BatChargeMon.ChargerMod3));
  break;
  case DRQ_BCM_MOD4:
    printf("Llegó DRQ_BCM_MOD4\r\n");
    sendSciMsg(DAT_BCM_MOD4, &BatChargeMon.ChargerMod4, sizeof(BatChargeMon.ChargerMod4));
  break;
  case DRQ_BCM_MOD5:
    printf("Llegó DRQ_BCM_MOD5\r\n");
    sendSciMsg(DAT_BCM_MOD5, &BatChargeMon.ChargerMod5, sizeof(BatChargeMon.ChargerMod5));
  break;
  case DRQ_BCM_MOD6:
    printf("Llegó DRQ_BCM_MOD6\r\n");
    sendSciMsg(DAT_BCM_MOD6, &BatChargeMon.ChargerMod6, sizeof(BatChargeMon.ChargerMod6));
  break;
  case DRQ_BCM_MOD7:
   printf("Llegó DRQ_BCM_MOD7\r\n");
   sendSciMsg(DAT_BCM_MOD7, &BatChargeMon.ChargerMod7, sizeof(BatChargeMon.ChargerMod7));
  break;
  case DRQ_BCM_MOD8:
   printf("Llegó DRQ_BCM_MOD8\r\n");
   sendSciMsg(DAT_BCM_MOD8, &BatChargeMon.ChargerMod8, sizeof(BatChargeMon.ChargerMod8));
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
