/**
 * Reads from the raindrop sensor
 * D2 and A0
 * 
 * For sleep -->
 * Connections:
 * D0 -- RST
 *
 * If you cant reprogram as the ESP is sleeping, disconnect D0 - RST and try again
 * From: https://github.com/wemos/D1_mini_Examples/blob/master/examples/04.Shields/DHT_Shield/DeepSleep/DeepSleep.ino
 */
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

const int sleepSeconds = 5;
const char* ssid = "SSID";
const char* password = "pass";

HTTPClient http;

void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  Serial.begin(230400);

  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //Serial.printf("Sleep for %d seconds\n\n", sleepSeconds);
  // convert to microseconds
  //ESP.deepSleep(sleepSeconds * 1000000);
}

void loop() {
  readSensor();
}

void readSensor() {
  bool isRaining = !digitalRead(D2);
  int intensity = (1024 - analogRead(A0)) * 100 / 1024;
  Serial.print("Lluvia: ");
  if (isRaining) {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("SI");
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print("NO");
  }
  Serial.print(" - Intensidad: " );
  Serial.print(intensity);
  Serial.println(" %");

  http.begin("http://192.168.1.99:8080/rain");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.POST("raining=" + String(isRaining) +"&intensity=" + String(intensity));
  //http.writeToStream(&Serial);
  http.end();
  Serial.println("Data sent to server");
  digitalWrite(LED_BUILTIN, HIGH);
}

