#include <SoftwareSerial.h>

// Configuración de pines para SoftwareSerial (HC-05)
#define BT_RX 11  // Pin RX del HC-05 → Arduino pin 11 (TX)
#define BT_TX 12  // Pin TX del HC-05 → Arduino pin 12 (RX)

SoftwareSerial BTSerial(BT_RX, BT_TX);  // RX, TX (desde la perspectiva del Arduino)

// Pines de conexión para el L298N
const int pinIN1_l = 6;  // Controla la dirección motor izq(sentido)
const int pinIN2_l = 7;  // Controla la dirección motor izq(sentido)
const int pinEN_l = 5;   // Controla la velocidad motor izq(PWM)

const int pinIN1_r = 8;  // Controla la dirección motor der(sentido)
const int pinIN2_r = 9;  // Controla la dirección motor der(sentido)
const int pinEN_r = 10;  // Controla la velocidad motor der(PWM)

const int buzzerPin = A5;  // Controla el buzzer

// Variable para almacenar la velocidad actual
int speed = 120;          // Velocidad media por defecto (rango 0-255)
int duracion_mov = 1000;  //duracion del movimiento cuando se selecciona avanzar, retroceder, izq o derecha

// Variables para control de tiempo
unsigned long lastMoveTime = 0;
bool isMoving = false;
char currentMove = 's';  // Estado inicial: detenido

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
  Serial.println("z - Sirena Ambulancia");
  Serial.println("a - Sirena Bomberos");
  Serial.println("y - Tocata y Fuga");
}

void loop() {
  // Control de tiempo para detención automática
  if (isMoving && (millis() - lastMoveTime >= duracion_mov)) {
    stopMotors();
    isMoving = false;
    Serial.println("Auto-detencion");
  }

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
  switch (command) {
    case 'f':  // Adelante
    case 'b':  // Atrás
    case 'l':  // Izquierda
    case 'r':  // Derecha
      startMovement(command);
      break;

    case 's':  // Detener
      stopMotors();
      isMoving = false;
      Serial.println("Motores detenidos");
      break;

    case '0' ... '9':  // Ajustar velocidad
      // Convertir el carácter a un valor entre 0 y 255
      speed = map(command - '0', 0, 9, 0, 255);
      setSpeed(speed);
      Serial.print("Velocidad ajustada a: ");
      Serial.println(speed);
      break;

    case 'z':  //sirenaAmbulancia
      sirenaAmbulancia();
      Serial.println("sirenaAmbulancia");
      break;

    case 'a':
      break;

    case 'y':
      break;
  }
}

void startMovement(char direction) {
  // Configura dirección
  switch (direction) {
    case 'f':
      digitalWrite(pinIN1_l, HIGH);
      digitalWrite(pinIN2_l, LOW);
      digitalWrite(pinIN1_r, HIGH);
      digitalWrite(pinIN2_r, LOW);
      Serial.println("Dirección: Adelante");
      break;

    case 'b':
      digitalWrite(pinIN1_l, LOW);
      digitalWrite(pinIN2_l, HIGH);
      digitalWrite(pinIN1_r, LOW);
      digitalWrite(pinIN2_r, HIGH);
      Serial.println("Dirección: Atrás");
      break;

    case 'l':
      digitalWrite(pinIN1_l, LOW);
      digitalWrite(pinIN2_l, HIGH);
      digitalWrite(pinIN1_r, HIGH);
      digitalWrite(pinIN2_r, LOW);
      Serial.println("Girando Izquierda");
      break;

    case 'r':
      digitalWrite(pinIN1_l, HIGH);
      digitalWrite(pinIN2_l, LOW);
      digitalWrite(pinIN1_r, LOW);
      digitalWrite(pinIN2_r, HIGH);
      Serial.println("Girando Derecha");
      break;
  }

  setSpeed(speed);
  isMoving = true;
  lastMoveTime = millis();  // Reinicia temporizador
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

void sirenaAmbulancia() {
  // 🚑 Sirena de ambulancia

  for (int freq = 650; freq <= 1000; freq += 10) {
    tone(buzzerPin, freq);
    delay(4);
  }
  for (int freq = 1000; freq >= 650; freq -= 10) {
    tone(buzzerPin, freq);
    delay(4);
  }
  noTone(buzzerPin);
  delay(1000);
}
