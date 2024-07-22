#include <WiFi.h>
#include "FirebaseESP32.h"

#define WIFI_SSID "Totalplay-23A6"
#define WIFI_PASSWORD "23A63F7DQFx967U8"

#define FIREBASE_HOST "espp32-771cb-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "jQ4C2UodDI6Irlzsodj1lOBJNt9CT2aN1fSDX5QC"

// Pins de los sensores
#define LM34_PIN 34 // LM34 conectado al pin 34
#define YL69_PIN 35 // YL69 conectado al pin 35

FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

String nodo = "/Sensores";

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Conectar a WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando al WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.println("Conectado al WiFi");

  // Configurar Firebase
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Leer el valor analógico del sensor LM34 (temperatura)
  int lm34_analogValue = analogRead(LM34_PIN);

  // Convertir el valor analógico del LM34 a temperatura en grados Celsius
  float lm34_voltage = lm34_analogValue * (3.3 / 4095.0);
  float temperatureC = ((lm34_voltage - 0.5) * 100.0)*-1;

  // Imprimir y enviar la temperatura a Firebase
  Serial.print("Temperatura: ");
  Serial.print(temperatureC);
  Serial.println(" °C");
  if (Firebase.setFloat(firebaseData, nodo + "/sensor1", temperatureC)) {
    Serial.println("Temperatura enviada correctamente a Firebase");
  } else {
    Serial.println("Error al enviar temperatura a Firebase: " + firebaseData.errorReason());
  }

  // Leer el valor analógico del sensor YL69 (humedad)
  int yl69_analogValue = analogRead(YL69_PIN);

  // Invertir la interpretación de la lectura del YL69 para humedad
  float humidityPercentage = map(yl69_analogValue, 4095, 0, 0, 100); // Invertir el mapeo

  // Imprimir y enviar la humedad a Firebase
  Serial.print("Humedad: ");
  Serial.print(humidityPercentage);
  Serial.println(" %");
  if (Firebase.setFloat(firebaseData, nodo + "/sensor2", humidityPercentage)) {
    Serial.println("Humedad enviada correctamente a Firebase");
  } else {
    Serial.println("Error al enviar humedad a Firebase: " + firebaseData.errorReason());
  }

  delay(2000); // Esperar 2 segundos antes de la siguiente lectura
}
