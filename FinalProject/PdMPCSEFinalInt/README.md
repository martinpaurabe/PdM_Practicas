Alumno: Martín Paura Bersan

Plataforma embebida: NUCLEO-STM32F401RE

VERSION INICIAL DEL PROYECTO

Aplicación:
Sistema centralizado que realiza el relevamiento del estado de carga de un pack de 8 baterías (Cada batería cuenta con un módulo que controla la carga y toma energía de un BUS de potencia DC, y puede ser consultado mediante protocolo UART) y muestra los datos obtenidos mediante una pantalla LCD de 20x4. En caso de estar las 8 baterías cargadas completamente informa al usuario que la carga se completó.
Contará con un pulsador para pasar por distintas pantallas y mostrar datos adicionales.

Periféricos (1 ó 2): 
UART, GPIO, I2C

Diagrama de estado de MEF con una breve descripción de cada estado.



Mostrar % Estado de Carga: Muestra el porcentaje de carga actual en la que se encuentra cada batería en el LCD de 20x4.
Mostrar PWM de Carga: Muestra el PWM actual que tiene cada controlador de carga de las baterías en el LCD de 20x4.
Carga Completa: Muestra un mensaje en el centro de la pantalla LCD indicando al usuario que todas las baterías se encuentran cargadas.

Definir los módulos de software (archivos) que va  implementar para cada periférico.
ejemplo:
API_FSMBatShow.c y API_FSMBatShow.h para manejar la maquina de estado del sistema 
API_LCD.h  y API_LCD.c  para el envío mensajes a la pantalla LCD.
API_BatChargers.c y API_BatChargers.h para consulta de información a los módulos de carga de baterías (Manejara el Puerto UART y el multiplexor de selección de módulo, por GPIO).
API_debounce.c y API_debounce.h Para el manejo del pulsador que permite el saltó entre las distintas páginas a mostrar en el LCD.

Definir los prototipos de las principales funciones públicas y privadas de cada módulo definido .
API_FSMBatShow:
typedef enum{ShowBatCharge,ShowBatPWM, ShowChargeCmplt} BarShowStates
bool_t FSMBatShow_init(); // inicializa la máquina de estado, el mensaje de LCD y Comunicación UART con Controladores de carga. Devuelve True si la inicialización fue exitosa, false en caso contrario.
void FSMBatShow_UpDate(); // Realiza la lectura de estado de carga de cada batería y si hubo una acción en el pulsador, Y verifica si tiene que modificar el estado de la maquina de estado.

API_LCD:
bool_t API_LCD_init(); // inicializa el LCD y el puerto I2C. Devuelve True si la inicialización fue exitosa, false en caso contrario.
void FSMBatShow_SendMsg(chart *Msg, uint8_t MsgLength, uint_8 MsgLine); // Imprime un mensaje en la línea indicada en el LCD.


API_BatChargers:
typedef enum{ChargeState, ChargePWM} BatChargDataReq
typedef enum{Charger1, Charger2,...,Charger8} ChargAddress
bool_t API_BatChargers_init(); // inicializa el puerto UART y el GPIO, para la comunicación con los controladores de carga y el multiplexor de los canales de comunicación. Devuelve True si la inicialización fue exitosa, false en caso contrario.
int32_t API_BatChargers_DataReq(ChargAddress SelectedCharger, BatChargDataReq DataReq); //Solicita la información requerida al controlador de carga de batería seleccionado.

