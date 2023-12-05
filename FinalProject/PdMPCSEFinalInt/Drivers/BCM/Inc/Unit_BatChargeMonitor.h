/*
 * Unit_BatChargeMonitor.h
 *
 *  Created on: Dec 3, 2023
 *      Author: martin
 */

#ifndef BCM_INC_UNIT_BATCHARGEMONITOR_H_
#define BCM_INC_UNIT_BATCHARGEMONITOR_H_

//---------------------------------------------------------------------------


#define BC_NUMBERS  8
#define BAT_DISCHRGD 50 //Percentage which define that the batteries need to be charged
#define BAT_CHRGD 	 80 //Percentage which define that the batteries is charged

//==============================================================================================================
// Protocol Constant Definition
//==============================================================================================================
#define BCM_FSM_PERIODE 100



//Comandos Sci
//*********************************
//Ordenes de habilitacion
#define CMD_BCM_SAVE             0x00
#define CMD_BCM_STATUS           0x01



//Charger Module 1 Orders
#define CMD_BCM_CMOD_1            0x20
#define CMD_BCM_CMOD_1_PWM        0x21
#define CMD_BCM_CMOD_1_TEN_DES    0x22
#define CMD_BCM_CMOD_1_COR_DES    0x23


//Charger Module 2 Orders
#define CMD_BCM_CMOD_2            0x30
#define CMD_BCM_CMOD_2_PWM        0x31
#define CMD_BCM_CMOD_2_TEN_DES    0x32
#define CMD_BCM_CMOD_2_COR_DES    0x33




#define CMD_BCM_GRAL              0x70


//Data Request
#define DRQ_BCM_MOD1           0x80
#define DRQ_BCM_MOD2           0x81
#define DRQ_BCM_MOD3           0x82
#define DRQ_BCM_MOD4           0x83
#define DRQ_BCM_MOD5           0x84
#define DRQ_BCM_MOD6           0x85
#define DRQ_BCM_MOD7           0x86
#define DRQ_BCM_MOD8           0x87


//Data Send
//Pedido de Datos
#define DAT_BCM_MOD1           0xA0
#define DAT_BCM_MOD2           0xA1
#define DAT_BCM_MOD3           0xA2
#define DAT_BCM_MOD4           0xA3
#define DAT_BCM_MOD5           0xA4
#define DAT_BCM_MOD6           0xA5
#define DAT_BCM_MOD7           0xA6
#define DAT_BCM_MOD8           0xA7


void BatChargMon_Init(void);
void BatChargMon_Update(void);
//---------------------------------------------------------------------------


#endif /* BCM_INC_UNIT_BATCHARGEMONITOR_H_ */
