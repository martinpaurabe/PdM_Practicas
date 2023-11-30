#ifndef DEF_MOTOR_H
#define DEF_MOTOR_H
//==============================================================================================================

#include "Def_TipoDatos.h"
#include "GPIO.h"

//==============================================================================================================
// DEFINICION DE CONSTANTES
//==============================================================================================================


#define DIM_NOMBRE_GES   12
#define PWM_MAX          600
#define PWM_MED          300
#define PWM_DTM	         200



#define HabOut48On      setRegBit(GPIO_A_DR,D6)
#define HabOut48Off     clrRegBit(GPIO_A_DR,D6)
#define HabAlimAuxOn    setRegBit(GPIO_A_DR,D7)
#define HabAlimAuxOff   clrRegBit(GPIO_A_DR,D7)
#define HabIntCircOn    setRegBit(GPIO_A_DR,D8)
#define HabIntCircOff   clrRegBit(GPIO_A_DR,D8)
#define HabBrkOn        setRegBit(GPIO_A_DR,D9)
#define HabBrkOff       clrRegBit(GPIO_A_DR,D9)
#define HabIn220On      setRegBit(GPIO_A_DR,D10)
#define HabIn220Off     clrRegBit(GPIO_A_DR,D10)
#define HabAeroGenOn    setRegBit(GPIO_A_DR,D11)
#define HabAeroGenOff   clrRegBit(GPIO_A_DR,D11)
#define HabPanSolarOn   setRegBit(GPIO_A_DR,D11)
#define HabPanSolarOff  clrRegBit(GPIO_A_DR,D11)
#define HabPWMCBAOn     PWMA_PWMVAL4 = 0;
#define HabPWMCBAOff    PWMA_PWMVAL4 = PWM_MAX;



#define LedPWMCBAOn      setRegBit(GPIO_B_DR,D4)
#define LedPWMCBAOff     clrRegBit(GPIO_B_DR,D4)
#define LedIntCircOn     setRegBit(GPIO_B_DR,D5)
#define LedIntCircOff    clrRegBit(GPIO_B_DR,D5)
#define LedOut48On       setRegBit(GPIO_B_DR,D6)
#define LedOut48Off      clrRegBit(GPIO_B_DR,D6)
#define LedBrkOn         setRegBit(GPIO_B_DR,D7)
#define LedBrkOff        clrRegBit(GPIO_B_DR,D7)
#define LedAeroGenOn     setRegBit(GPIO_C_DR,D2)
#define LedAeroGenOff    clrRegBit(GPIO_C_DR,D2)
#define LedPanSolarOn    setRegBit(GPIO_C_DR,D3)
#define LedPanSolarOff   clrRegBit(GPIO_C_DR,D3)
#define LedIn220On       setRegBit(GPIO_C_DR,D4)
#define LedIn220Off      clrRegBit(GPIO_C_DR,D4)




//Comandos Sci
//*********************************
//Ordenes de habilitacion
#define CMD_BCM_SAVE             0x00
#define CMD_BCM_STATUS           0x01
#define CMD_BCM_STT_LED2_ON      0x02
#define CMD_BCM_STT_LED2_OFF     0x03



//Ordenes para los Módulos de Carga
#define CMD_BCM_CBA            0x20
#define CMD_BCM_CBA_PWM        0x21
#define CMD_BCM_CBA_TEN_DES    0x22
#define CMD_BCM_CBA_COR_DES    0x23
#define CMD_BCM_CBA_SEGUR      0x24
#define CMD_BCM_CBA_CTRL_PID   0x25
#define CMD_BCM_CBA_CTRL_ON    0x26
#define CMD_BCM_CBA_CTRL_OFF   0x27


//Ordenes para Modulo de Carga Numero 1
#define CMD_BCM_CBA1            0x30
#define CMD_BCM_CBA1_PWM        0x31
#define CMD_BCM_CBA1_TEN_DES    0x32
#define CMD_BCM_CBA1_COR_DES    0x33




#define CMD_BCM_GRAL              0x70


//Pedido de Datos
#define DRQ_GES                0x80
#define DRQ_BCM_GRAL           0x81
#define DRQ_BCM_CBA            0x82
#define DRQ_BCM_IN220          0x83
#define DRQ_BCM_OUT220         0x84
#define DRQ_BCM_BRK            0x85

//Datos
#define DAT_BCM_SET            0xA0
#define DAT_BCM_SET_GRAL       0xA1
#define DAT_BCM_SET_CBA        0xA2
#define DAT_BCM_SET_IN220      0xA3
#define DAT_BCM_SET_OUT220     0xA4
#define DAT_BCM_SET_BRK        0xA5


//==============================================================================================================
// DEFINICION DE TIPOS DE VARIABLES
//==============================================================================================================


typedef struct
{
  char Nombre[DIM_NOMBRE_GES];
} TConstGestEnergia;


typedef struct
{
  byte seg;
  byte min;
  byte hor;
  byte DummyOn;
} TTimeDistri;


typedef union
{
  TTimeDistri hms;
  TDato32 ttl;
}TypTime;

typedef struct
{
  TConstGestEnergia Const;
  TypTime Time;
  TypTime TimeIniSolar;
  TypTime TimeFinSolar;
  TDato32 Status;
}TGralGestEnergia;


typedef struct
{
  float HH;
  float H;
  float L;
  float LL;
} TParaSafe;


typedef struct
{
  float Prop;
  float Int;
  float Der;
} TCtrl;

typedef struct
{
	float Des;
	float Act;
	float Error;
	TCtrl Ctrl;
	TParaSafe Safe;
	
}TVarCtrl;

typedef struct
{
	float   Des;
	float   Max;
	float   Min;
	float   DeltaCarga;
	TDato32 TimeSen;
	TDato32 TimeCarg;
}TPWM;

typedef struct
{
  TPWM      PWM;
  TVarCtrl  Ten;
  TVarCtrl  Cor;
} TVcc;

typedef struct
{
  TVarCtrl  Freq;
  TDato32   PPT;
  TDato32   Ang;
  TDato32   Fase;
  TVarCtrl  Ten;
  TVarCtrl  Cor;
} TVac;





typedef struct
{
  TGralGestEnergia Gral;
  TVcc    CargBat;
  TVcc    In220;
  TVac    Out220;
  TVac    Brk;
  
//Definicion de Salidas
#define STAT_BCM_AEG             (0x00000001<<0)
#define STAT_BCM_SOL             (0x00000001<<1)
#define STAT_BCM_CBA             (0x00000001<<2)
#define STAT_BCM_CIN             (0x00000001<<3)
#define STAT_BCM_OUT48           (0x00000001<<4)
#define STAT_BCM_BRK             (0x00000001<<5)
#define STAT_BCM_IN220           (0x00000001<<6)
#define STAT_BCM_OUT220          (0x00000001<<7)
#define STAT_BCM_ALAUX           (0x00000001<<8)

//Definicion de Estados Internos
#define STAT_BCM_CBA_AUTO        (0x00000001<<10)

} TGestEnergia;

//==============================================================================================================
// DECLARACION DE FUNCIONES
//==============================================================================================================

void Inc_Timer(void);
void Ctrl_GestEnergia(void);


//==============================================================================================================
#endif  //End of DEF_MOTOR_H//
