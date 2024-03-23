#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <RF24.h>

const char* ssid = "TIGO-FC99"; 
const char* password = "2NB144203115"; // Reemplaza con tu contraseña
const char* server = "http://telemsebas-0a07559143b3.herokuapp.com/subirdatos.php"; 
RF24 radio(2, 4);  // Utiliza los pines D2 y D4 para CE y CSN respectivamente

struct DataPacket {
  float humidity1;
  float temperature1;
  float humidity2;
  float temperature2;
};

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");

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

    // Crear cliente HTTP para la solicitud POST al servidor
    WiFiClient client;
    HTTPClient http;

    String url = String(server);
    http.begin(client, url);

    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Ajustar el nombre del campo y la asignación según tu base de datos
    String postBody = "&humedad_1=" + String(dataPacket.humidity1) +
                      "&temperatura_1=" + String(dataPacket.temperature1) +
                      "&humedad_2=" + String(dataPacket.humidity2) +
                      "&temperatura_2=" + String(dataPacket.temperature2);

    int httpCode = http.POST(postBody);

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Server Answer:");
      Serial.println(payload);
    } else {
      Serial.printf("Error en la solicitud HTTP %d\n", httpCode);
    }

    http.end();
  }
}
