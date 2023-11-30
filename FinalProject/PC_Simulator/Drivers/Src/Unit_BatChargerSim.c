#include "main.h"

#include <stdio.h>
#include <string.h>

#include "Unit_Disp.h"



//--------------------------------------------------------------------------------------------------------------


TBatChargeMon BatChargeMon;


const TBatChargeMon BatChargeMonFlash;


void Init_Disp(void)
{
  BatChargeMon=BatChargeMonFlash;
 
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
    sendSciMsg(DRQ_BCM_MOD1, &BatChargeMon.ChargerMod1, sizeof(BatChargeMon.ChargerMod1));
  break;
  case DRQ_BCM_MOD2:
    sendSciMsg(DRQ_BCM_MOD2, &BatChargeMon.ChargerMod2, sizeof(BatChargeMon.ChargerMod2));
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

