#include <Wire.h>
#include <BH1750.h>
#include <WiFiNINA.h>

// WiFi credentials
char ssid[] = "S22Nisal";
char pass[] = "nisal2004";

// IFTTT Webhooks configuration
char HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME = "https://maker.ifttt.com/trigger/light_readings/with/key/PwZS4Xw-FXkukWG8TRCaV"; // Updated Webhooks URL
String queryString = "?value1="; // Query parameters for light readings

WiFiClient client;

BH1750 lightMeter;

void setup() {
  
  Serial.begin(9600);
  while (!Serial);

  // Initialize the I2C bus and BH1750 sensor
  Wire.begin();
  lightMeter.begin();

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // Connect to IFTTT server
  if (client.connect(HOST_NAME, 80)) {
    Serial.println("Connected to IFTTT server");
  } else {
    Serial.println("Connection to IFTTT server failed");
  }
}

void loop() {
  // Read light level from BH1750 sensor
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  // Send HTTP request to IFTTT with light readings
  sendIFTTTEvent(PATH_NAME + queryString + String(lux));

  delay(1000); // Send readings every  second

}

void sendIFTTTEvent(String path) {
  // Send HTTP request to IFTTT Webhooks
  if (client.connected()) {
    client.println("GET " + path + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
   // client.println("Connection: close");
    client.println();
    Serial.println("HTTP GET request sent to IFTTT with light readings");
  } else {
    Serial.println("Failed to send HTTP GET request to IFTTT");
  }
}
