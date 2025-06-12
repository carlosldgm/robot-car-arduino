#include <SoftwareSerial.h>

// Configuración de pines para SoftwareSerial (HC-05)
#define BT_RX 11  // Pin RX del HC-05 → Arduino pin 11 (TX)
#define BT_TX 12  // Pin TX del HC-05 → Arduino pin 12 (RX)

SoftwareSerial BTSerial(BT_RX, BT_TX); // RX, TX (desde la perspectiva del Arduino)

// Pines de conexión para el L298N
const int pinIN1_l = 6;    // Controla la dirección motor izq(sentido)
const int pinIN2_l = 7;    // Controla la dirección motor izq(sentido)
const int pinEN_l = 5;     // Controla la velocidad motor izq(PWM)

const int pinIN1_r = 8;    // Controla la dirección motor der(sentido)
const int pinIN2_r = 9;    // Controla la dirección motor der(sentido)
const int pinEN_r = 10;    // Controla la velocidad motor der(PWM)

// Variable para almacenar la velocidad actual
int speed = 120;  // Velocidad media por defecto (rango 0-255)

void setup() {
  // Inicializa comunicación con la PC (Monitor Serial)
  Serial.begin(9600);
  
  // Inicializa comunicación con el HC-05
  BTSerial.begin(9600);  // Asegúrate de que coincide con la velocidad del HC-05
  
  // Configurar los pines de motores como salidas
  pinMode(pinIN1_l, OUTPUT);
  pinMode(pinIN2_l, OUTPUT);
  pinMode(pinEN_l, OUTPUT);

  pinMode(pinIN1_r, OUTPUT);
  pinMode(pinIN2_r, OUTPUT);
  pinMode(pinEN_r, OUTPUT);
  
  // Inicializar el motor detenido
  stopMotors();
  
  Serial.println("Sistema listo - Control via Bluetooth habilitado");
  Serial.println("Comandos disponibles:");
  Serial.println("f - Mover hacia adelante");
  Serial.println("b - Mover hacia atrás");
  Serial.println("r - Girar a la derecha");
  Serial.println("l - Girar a la izquierda");
  Serial.println("s - Detener motores");
  Serial.println("0-9 - Ajustar velocidad (0=min, 9=max)");
}

void loop() {
  // Leer datos del Bluetooth
  if (BTSerial.available()) {
    char command = BTSerial.read();
    Serial.print("BT: ");  // Mostrar en monitor serial
    Serial.println(command);
    executeCommand(command);
  }

  // Leer datos del Monitor Serial
  if (Serial.available()) {
    char command = Serial.read();
    executeCommand(command);
  }
  
  // Pequeña pausa para estabilidad
  delay(10);
}

// Función para ejecutar comandos
void executeCommand(char command) {
  switch(command) {
    case 'f': // Adelante (ambos motores)
      digitalWrite(pinIN1_l, HIGH);
      digitalWrite(pinIN2_l, LOW);
      digitalWrite(pinIN1_r, HIGH);
      digitalWrite(pinIN2_r, LOW);
      setSpeed(speed);
      Serial.println("Dirección: Adelante");
      break;
      
    case 'b': // Atrás
      digitalWrite(pinIN1_l, LOW);
      digitalWrite(pinIN2_l, HIGH);
      digitalWrite(pinIN1_r, LOW);
      digitalWrite(pinIN2_r, HIGH);
      setSpeed(speed);
      Serial.println("Dirección: Atrás");
      break;
      
    case 's': // Detener
      stopMotors();
      Serial.println("Motores detenidos");
      break;
      
    case 'l': // Izquierda
      // Motor izquierdo atrás, motor derecho adelante
      digitalWrite(pinIN1_l, LOW);
      digitalWrite(pinIN2_l, HIGH);
      digitalWrite(pinIN1_r, HIGH);
      digitalWrite(pinIN2_r, LOW);
      setSpeed(speed);
      Serial.println("Girando Izquierda");
      break;

    case 'r': // Derecha
      // Motor izquierdo adelante, motor derecho atrás
      digitalWrite(pinIN1_l, HIGH);
      digitalWrite(pinIN2_l, LOW);
      digitalWrite(pinIN1_r, LOW);
      digitalWrite(pinIN2_r, HIGH);
      setSpeed(speed);
      Serial.println("Girando Derecha");
      break;

    case '0'...'9': // Ajustar velocidad
      // Convertir el carácter a un valor entre 0 y 255
      speed = map(command - '0', 0, 9, 0, 255);
      setSpeed(speed);
      Serial.print("Velocidad ajustada a: ");
      Serial.println(speed);
      break;
  }
}

// Función para detener los motores
void stopMotors() {
  digitalWrite(pinIN1_l, LOW);
  digitalWrite(pinIN2_l, LOW);
  digitalWrite(pinIN1_r, LOW);
  digitalWrite(pinIN2_r, LOW);
  analogWrite(pinEN_l, 0);
  analogWrite(pinEN_r, 0);
}

// Función para establecer la velocidad de ambos motores
void setSpeed(int spd) {
  analogWrite(pinEN_l, spd);
  analogWrite(pinEN_r, spd);
}