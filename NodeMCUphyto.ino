#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#include <Wire.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT21  // DHT 21
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "TP-LINK_CA2988";  //ssid
const char* password = "******";      //password
const char* serverName = "http://www.rptronic.lk/post-esp-data.php";

String apiKeyValue = "tPmAT5Ab3j7F9";  //api key

float humidity, temperature;

int sensor_pin = A0;
int output_value;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  dht.begin();
}

void loop() {
  Read_h_t();
  soilMoisture();
  dataUpload();
}

void Read_h_t() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  Serial.println(humidity);
}

void soilMoisture() {
  output_value = analogRead(sensor_pin);
  delay(1000);  // Keep this delay to stabilize the analog reading
}

void dataUpload() {
  // Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Start connection to server
    http.begin(serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepare HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue +
                             "&humi=" + String(humidity) +
                             "&temp=" + String(temperature) +
                             "&mosit=" + String(output_value) +
                             "&light=" + "50";  // Replace "50" with actual light sensor value if available

    Serial.println("HTTP Request Data: " + httpRequestData);

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      // Optional: Print the server response for debugging
      String response = http.getString();
      Serial.println("Server Response: " + response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  // Delay for 180 seconds (3 minutes)
  delay(180000);
}
