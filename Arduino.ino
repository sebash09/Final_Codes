#include <RF24.h>

RF24 radio(2, 4);  // Pines D2 y D4 para CE y CSN respectivamente

struct DataPacket {
  float humidity1;
  float temperature1;
  float humidity2;
  float temperature2;
};

void setup() {
  Serial.begin(9600);
  
  if (!radio.begin()) {
    Serial.println("Error al iniciar la radio NRF24");
    while (1);
  }

  radio.openReadingPipe(1, 0xF0F0F0F0E1LL); // Dirección del canal de comunicación
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    DataPacket dataPacket;
    radio.read(&dataPacket, sizeof(dataPacket));

    Serial.print("Lecturas del sensor 1: ");
    Serial.print("Humedad: ");
    Serial.print(dataPacket.humidity1);
    Serial.print("%, Temperatura: ");
    Serial.print(dataPacket.temperature1);
    Serial.println("°C");

    Serial.print("Lecturas del sensor 2: ");
    Serial.print("Humedad: ");
    Serial.print(dataPacket.humidity2);
    Serial.print("%, Temperatura: ");
    Serial.print(dataPacket.temperature2);
    Serial.println("°C");
  }
}
