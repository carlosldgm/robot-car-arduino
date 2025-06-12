# robot-car-arduino
Código para controlar un auto robot con Arduino

----------------control de motores on bluetooth-----------------
Características del código integrado:

    Control dual: Funciona tanto con comandos Bluetooth como con el Monitor Serial

    Comandos mejorados:

        'f': Adelante

        'r': Atrás

        'i': Izquierda (i de "izquierda")

        'd': Derecha

        's': Detener

        '0'-'9': Control de velocidad (0 = mínimo, 9 = máximo)

    Funciones útiles:

        stopMotors(): Detiene ambos motores inmediatamente

        setSpeed(int spd): Establece la velocidad para ambos motores

        executeCommand(char command): Procesa los comandos recibidos

    Retroalimentación:

        Todos los comandos recibidos por Bluetooth se muestran en el Monitor Serial

        Se confirma cada acción ejecutada

Conexiones recomendadas:
Módulo HC-05:
HC-05	Arduino
TX	Pin 11
RX	Pin 12
GND	GND
VCC	5V
Driver L298N:
L298N	Arduino	Motores
ENB	Pin 10	Motor Derecho
IN3	Pin 8	Motor Derecho
IN4	Pin 9	Motor Derecho
ENA	Pin 5	Motor Izquierdo
IN1	Pin 6	Motor Izquierdo
IN2	Pin 7	Motor Izquierdo

Nota: Los pines de los motores deben conectarse a las salidas del L298N según corresponda.

Para controlar el vehículo, puedes usar cualquier aplicación Bluetooth que envíe caracteres simples (como "f", "r", "i", "d", "s" y números del 0-9)
