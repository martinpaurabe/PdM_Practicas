//#include <iostream>
//#include <stdio.h>


//My Libraries
#include "Unit_Disp.h"

enum {PARSER_DIRECTION, PARSER_PORCENTAJE};
enum {MODULO0,MODULO1,MODULO2,MODULO3,MODULO4,MODULO5,MODULO6,MODULO7,MODULO8};

using namespace std;

int main()
{
 
   // Allocate memory for read buffer, set size according to your needs
  char read_buf [256];
  ThreadComPort_Init();
  // Normally you wouldn't do this memset() call, but since we will just receive
  // ASCII data for this example, we'll set everything to 0 so we can
  // call printf() easily.
  memset(&read_buf, '\0', sizeof(read_buf));
  int num_Modulo=1;
  int PorcentSim=0;
  int STT = PARSER_DIRECTION;
  bool IsFirst=true;
  while(1)
  {
//    usleep(10000);
    ThreadComPort_Update();
    switch (STT)
    {
    case PARSER_DIRECTION:
      if(IsFirst)
      {
        IsFirst = false;
        printf("\r                                                                 ");
        printf("\rIngrese Número de Modulo a Modificar %% de Carga: ");
      } 
      if(num_Modulo = getchar())
      {
        num_Modulo -='0';
        IsFirst = true;
        if(1<=num_Modulo && num_Modulo<=8)
        {
          STT = PARSER_PORCENTAJE;
          getchar();
        }
        
      }
    break;
    case PARSER_PORCENTAJE:
     if(IsFirst)
      {
        IsFirst = false;
        printf("\r                                                                 ");
        printf("\rIngrese Porcentaje de Carga Simulado: ");
      } 
      if(PorcentSim = getchar())
      {
        PorcentSim-='0';
        PorcentSim*=10;
        IsFirst = true;
        if(0<=PorcentSim && PorcentSim<=100)
        {
          switch (num_Modulo)
          {
            case MODULO1:
              BatChargeMon.ChargerMod1.Porc = PorcentSim;
              BatChargeMon.ChargerMod1.Curr = 10.0*(100.0-PorcentSim)/100.0;
              BatChargeMon.ChargerMod1.PWM = (100.0-PorcentSim)/100.0;
              sendSciMsg(DAT_BCM_MOD1, &BatChargeMon.ChargerMod1, sizeof(BatChargeMon.ChargerMod1));
            break;
            case MODULO2:
              BatChargeMon.ChargerMod2.Porc = PorcentSim;
              BatChargeMon.ChargerMod2.Curr = 10.0*(100.0-PorcentSim)/100.0;
              BatChargeMon.ChargerMod2.PWM = (100.0-PorcentSim)/100.0;
              sendSciMsg(DAT_BCM_MOD2, &BatChargeMon.ChargerMod2, sizeof(BatChargeMon.ChargerMod2));
            break;
            case MODULO3:
              BatChargeMon.ChargerMod3.Porc = PorcentSim;
              BatChargeMon.ChargerMod3.Curr = 10.0*(100.0-PorcentSim)/100.0;
              BatChargeMon.ChargerMod3.PWM = (100.0-PorcentSim)/100.0;
              sendSciMsg(DAT_BCM_MOD3, &BatChargeMon.ChargerMod3, sizeof(BatChargeMon.ChargerMod3));
            break;
            case MODULO4:
              BatChargeMon.ChargerMod4.Porc = PorcentSim;
              BatChargeMon.ChargerMod4.Curr = 10.0*(100.0-PorcentSim)/100.0;
              BatChargeMon.ChargerMod4.PWM = (100.0-PorcentSim)/100.0;
              sendSciMsg(DAT_BCM_MOD4, &BatChargeMon.ChargerMod4, sizeof(BatChargeMon.ChargerMod4));
            break;
            case MODULO5:
              BatChargeMon.ChargerMod5.Porc = PorcentSim;
              BatChargeMon.ChargerMod5.Curr = 10.0*(100.0-PorcentSim)/100.0;
              BatChargeMon.ChargerMod5.PWM = (100.0-PorcentSim)/100.0;
             sendSciMsg(DAT_BCM_MOD5, &BatChargeMon.ChargerMod5, sizeof(BatChargeMon.ChargerMod5));
            break;
            case MODULO6:
              BatChargeMon.ChargerMod6.Porc = PorcentSim;
              BatChargeMon.ChargerMod6.Curr = 10.0*(100.0-PorcentSim)/100.0;
              BatChargeMon.ChargerMod6.PWM = (100.0-PorcentSim)/100.0;
              sendSciMsg(DAT_BCM_MOD6, &BatChargeMon.ChargerMod6, sizeof(BatChargeMon.ChargerMod6));
            break;
            case MODULO7:
              BatChargeMon.ChargerMod7.Porc = PorcentSim;
              BatChargeMon.ChargerMod7.Curr = 10.0*(100.0-PorcentSim)/100.0;
              BatChargeMon.ChargerMod7.PWM = (100.0-PorcentSim)/100.0;
              sendSciMsg(DAT_BCM_MOD7, &BatChargeMon.ChargerMod7, sizeof(BatChargeMon.ChargerMod7));
            break;
            case MODULO8:
              BatChargeMon.ChargerMod8.Porc = PorcentSim;
              BatChargeMon.ChargerMod8.Curr = 10.0*(100.0-PorcentSim)/100.0;
              BatChargeMon.ChargerMod8.PWM = (100.0-PorcentSim)/100.0;
              sendSciMsg(DAT_BCM_MOD8, &BatChargeMon.ChargerMod8, sizeof(BatChargeMon.ChargerMod8));
            break;
            default:
          
            break;
          }
          STT = PARSER_DIRECTION;
        }
      }

    default:
    break;
    }
  }    
       
  return 0; // success
};

