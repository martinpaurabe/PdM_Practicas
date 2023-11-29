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
#define CMD_GES_SAVE             0x00
#define CMD_GES_STATUS           0x01
#define CMD_GES_STT_AEG_ON       0x02
#define CMD_GES_STT_AEG_OFF      0x03
#define CMD_GES_STT_SOL_ON       0x04
#define CMD_GES_STT_SOL_OFF      0x05
#define CMD_GES_STT_CBA_ON       0x06
#define CMD_GES_STT_CBA_OFF      0x07
#define CMD_GES_STT_CINT_ON      0x08
#define CMD_GES_STT_CINT_OFF     0x09
#define CMD_GES_STT_OUT48_ON     0x0A
#define CMD_GES_STT_OUT48_OFF    0x0B
#define CMD_GES_STT_BRK_ON       0x0C
#define CMD_GES_STT_BRK_OFF      0x0D
#define CMD_GES_STT_IN220_ON     0x0E
#define CMD_GES_STT_IN220_OFF    0x0F
#define CMD_GES_STT_OUT220_ON    0x10
#define CMD_GES_STT_OUT220_OFF   0x11
#define CMD_GES_STT_ALAUX_ON     0x12
#define CMD_GES_STT_ALAUX_OFF    0x13



//Ordenes para la Carga de Bateria
#define CMD_GES_CBA            0x20
#define CMD_GES_CBA_PWM        0x21
#define CMD_GES_CBA_TEN_DES    0x22
#define CMD_GES_CBA_COR_DES    0x23
#define CMD_GES_CBA_SEGUR      0x24
#define CMD_GES_CBA_CTRL_PID   0x25
#define CMD_GES_CBA_CTRL_ON    0x26
#define CMD_GES_CBA_CTRL_OFF   0x27


//Ordenes para Entrada 220 Linea
#define CMD_GES_IN220            0x30
#define CMD_GES_IN220_PWM        0x31
#define CMD_GES_IN220_TEN_DES    0x32
#define CMD_GES_IN220_COR_DES    0x33
#define CMD_GES_IN220_SEGUR      0x34
#define CMD_GES_IN220_CTRL_PID   0x35
#define CMD_GES_IN220_CTRL_ON    0x36
#define CMD_GES_IN220_CTRL_OFF   0x37


//Ordenes para la salida a 220V
#define CMD_GES_OUT220              0x40
#define CMD_GES_OUT220_FREQ         0x41
#define CMD_GES_OUT220_TEN_DES      0x42
#define CMD_GES_OUT220_COR_DES      0x43
#define CMD_GES_OUT220_SEGUR        0x44
#define CMD_GES_OUT220_CTRL_PID     0x45
#define CMD_GES_OUT220_CTRL_ON      0x46
#define CMD_GES_OUT220_CTRL_OFF     0x47

//Ordenes para el Freno
#define CMD_GES_BRK              0x50
#define CMD_GES_BRK_FREQ         0x51
#define CMD_GES_BRK_TEN_DES      0x52
#define CMD_GES_BRK_COR_DES      0x53
#define CMD_GES_BRK_SEGUR        0x54
#define CMD_GES_BRK_CTRL_PID     0x55
#define CMD_GES_BRK_CTRL_ON      0x56
#define CMD_GES_BRK_CTRL_OFF     0x57


#define CMD_GES_GRAL              0x70


//Pedido de Datos
#define DRQ_GES                0x80
#define DRQ_GES_GRAL           0x81
#define DRQ_GES_CBA            0x82
#define DRQ_GES_IN220          0x83
#define DRQ_GES_OUT220         0x84
#define DRQ_GES_BRK            0x85

//Datos
#define DAT_GES_SET            0xA0
#define DAT_GES_SET_GRAL       0xA1
#define DAT_GES_SET_CBA        0xA2
#define DAT_GES_SET_IN220      0xA3
#define DAT_GES_SET_OUT220     0xA4
#define DAT_GES_SET_BRK        0xA5


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
#define STAT_GES_AEG             (0x00000001<<0)
#define STAT_GES_SOL             (0x00000001<<1)
#define STAT_GES_CBA             (0x00000001<<2)
#define STAT_GES_CIN             (0x00000001<<3)
#define STAT_GES_OUT48           (0x00000001<<4)
#define STAT_GES_BRK             (0x00000001<<5)
#define STAT_GES_IN220           (0x00000001<<6)
#define STAT_GES_OUT220          (0x00000001<<7)
#define STAT_GES_ALAUX           (0x00000001<<8)

//Definicion de Estados Internos
#define STAT_GES_CBA_AUTO        (0x00000001<<10)

} TGestEnergia;

//==============================================================================================================
// DECLARACION DE FUNCIONES
//==============================================================================================================

void Inc_Timer(void);
void Ctrl_GestEnergia(void);


//==============================================================================================================
#endif  //End of DEF_MOTOR_H//
