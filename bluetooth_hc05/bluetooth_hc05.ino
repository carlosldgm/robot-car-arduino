#include <SoftwareSerial.h>

// Configuración de pines para SoftwareSerial (HC-05)
#define BT_RX 11  // Pin RX del HC-05 → Arduino pin 11 (TX)
#define BT_TX 12  // Pin TX del HC-05 → Arduino pin 12 (RX)

SoftwareSerial BTSerial(BT_RX, BT_TX); // RX, TX (desde la perspectiva del Arduino)

void setup() {
  // Inicializa comunicación con la PC (Monitor Serial)
  Serial.begin(9600);
  
  // Inicializa comunicación con el HC-05
  BTSerial.begin(9600);  // Asegúrate de que coincide con la velocidad del HC-05
  
  Serial.println("Listo para comunicar con HC-05 (Pines 11 y 12)");
}

void loop() {
  // **Si hay datos desde el PC (Serial) → enviar al HC-05 (BTSerial)**
  if (Serial.available()) {
    char c = Serial.read();
    BTSerial.write(c);
  }

  // **Si hay datos desde el HC-05 (BTSerial) → enviar al PC (Serial)**
  if (BTSerial.available()) {
    char c = BTSerial.read();
    Serial.write(c);
  }
}