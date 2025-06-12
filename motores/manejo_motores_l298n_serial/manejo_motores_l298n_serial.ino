// Pines de conexión para el L298N
const int pinIN1_l = 6;    // Controla la dirección motor izq(sentido)
const int pinIN2_l = 7;    // Controla la dirección motor izq(sentido)
const int pinEN_l = 5;    // Controla la velocidad motor izq(PWM)

const int pinIN1_r = 8;    // Controla la dirección motor der(sentido)
const int pinIN2_r = 9;    // Controla la dirección motor der(sentido)
const int pinEN_r = 10;    // Controla la velocidad motor der(PWM)

void setup() {
  // Configurar los pines como salidas
  pinMode(pinIN1_l, OUTPUT);
  pinMode(pinIN2_l, OUTPUT);
  pinMode(pinEN_l, OUTPUT);

  pinMode(pinIN1_r, OUTPUT);
  pinMode(pinIN2_r, OUTPUT);
  pinMode(pinEN_r, OUTPUT);
  
  // Inicializar el motor detenido
  digitalWrite(pinIN1_l, LOW);
  digitalWrite(pinIN2_l, LOW);
  analogWrite(pinEN_l, 0);

  digitalWrite(pinIN1_r, LOW);
  digitalWrite(pinIN2_r, LOW);
  analogWrite(pinEN_r, 0);
  
  // Iniciar comunicación serial para depuración
  Serial.begin(9600);
  Serial.println("Control de motor DC con L298N");
  Serial.println("Comandos:");
  Serial.println("f - mover hacia adelante");
  Serial.println("r - mover hacia atrás");
  Serial.println("d - mover hacia la derecha");
  Serial.println("l - mover hacia la izq");
  Serial.println("s - detener motores");
  Serial.println("0-9 - ajustar velocidad (0=min, 9=max)");
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    
    switch(command) {
      case 'f': // Adelante (mueve ambos motores)
        digitalWrite(pinIN1_l, HIGH);
        digitalWrite(pinIN2_l, LOW);
        digitalWrite(pinIN1_r, HIGH);
        digitalWrite(pinIN2_r, LOW);
        Serial.println("Dirección: Adelante");
        break;
        
      case 'r': // Atrás
        digitalWrite(pinIN1_l, LOW);
        digitalWrite(pinIN2_l, HIGH);
        digitalWrite(pinIN1_r, HIGH);
        digitalWrite(pinIN2_r, LOW);        
        Serial.println("Dirección: Atrás");
        break;
        
      case 's': // Detener
        digitalWrite(pinIN1_l, LOW);
        digitalWrite(pinIN2_l, LOW);
        digitalWrite(pinIN1_r, LOW);
        digitalWrite(pinIN2_r, LOW);        
        Serial.println("Motores detenidos");
        break;
        
      case 'l': // Izquierda (Girar izquierda (motor derecho avanza, izquierdo detenido))
        digitalWrite(pinIN1_l, LOW);
        digitalWrite(pinIN2_l, LOW);
        digitalWrite(pinIN1_r, HIGH);
        digitalWrite(pinIN2_r, LOW);        
        Serial.println("Girando Izquierda");
        break;

      case 'd': // Girar derecha (motor izquierdo avanza, derecho detenido)
        digitalWrite(pinIN1_l, HIGH);
        digitalWrite(pinIN2_l, LOW);
        digitalWrite(pinIN1_r, LOW);
        digitalWrite(pinIN1_r, LOW);
        Serial.println("Girando derecha");
        break;        



      case '0'...'9': // Ajustar velocidad
        // Convertir el carácter a un valor entre 0 y 255
        int speed = map(command - '0', 0, 9, 0, 255);
        analogWrite(pinEN_l, speed);
        analogWrite(pinEN_r, speed);
        Serial.print("Velocidad ajustada a: ");
        Serial.println(speed);
        break;

      case 'p': // Adelante (mueve ambos motores)
        digitalWrite(pinIN1_r, HIGH);
        digitalWrite(pinIN2_r, LOW);
        Serial.println("Dirección: Adelante");
        analogWrite(pinEN_r, speed);
        Serial.print("Prueba rueda l: ");
        Serial.println(speed);
        break;        
    }
  }
}