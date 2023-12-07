Alumno: Martín Paura Bersan

Plataforma embebida: NUCLEO-STM32F401RE

Aplicación:
Sistema centralizado que realiza el relevamiento del estado de carga de un pack de 8 baterías 
(Cada batería cuenta con un módulo que controla la carga y toma energía de un BUS de potencia DC,
y puede ser consultado mediante protocolo UART) y muestra los datos obtenidos mediante una 
pantalla LCD de 20x4. En caso de estar las 8 baterías cargadas completamente informa al usuario 
que la carga se completó.
Contará con un pulsador para pasar por distintas pantallas y mostrar datos adicionales.

Periféricos (1 ó 2): 
UART, GPIO, I2C

Diagrama de estado de MEF con una breve descripción de cada estado.


