



void setup() {
  // Inicializa comunicación con PC
  Serial.begin(9600);
  Serial.println("Listo para comunicar con HC-05");

  // Inicializa comunicación con el HC-05
  Serial.begin(9600); // Velocidad predeterminada del HC-05
}

void loop() {
  // Si hay datos desde el monitor serie, envíalos al HC-05
  if (Serial.available()) {
    char c = Serial.read();
    Serial.write(c);
  }

  // Si hay datos desde el HC-05, envíalos al monitor serie
  if (Serial.available()) {
    char c = Serial.read();
    Serial.write(c);
  }
}
