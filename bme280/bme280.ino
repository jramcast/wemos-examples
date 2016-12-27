/*
BME280 I2C Test.ino 

Mas info aqui: https://github.com/TheAustrian/Wemos-D1-Mini-BME280-Weather-Station

Connecting the BME280 Sensor:
Sensor              ->  Board
-----------------------------
Vin (Voltage In)    ->  3.3V
Gnd (Ground)        ->  Gnd
SDA (Serial Data)   ->  A4 on Uno/Pro-Mini, 20 on Mega2560/Due, 2 Leonardo/Pro-Micro
SCK (Serial Clock)  ->  A5 on Uno/Pro-Mini, 21 on Mega2560/Due, 3 Leonardo/Pro-Micro

 */

/* ==== Includes ==== */
#include <BME280I2C.h>      // https://github.com/finitespace/BME280
#include <Wire.h>             // Needed for legacy versions of Arduino.
/* ====  END Includes ==== */

/* ==== Defines ==== */
#define SERIAL_BAUD 115200
/* ==== END Defines ==== */

/* ==== Global Variables ==== */
BME280I2C bme;                   // Default : forced mode, standby time = 1000 ms
                              // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,
bool metric = true;
/* ==== END Global Variables ==== */


/* ==== Prototypes ==== */
/* === Print a message to stream with the temp, humidity and pressure. === */
void printBME280Data(Stream * client);
/* === Print a message to stream with the altitude, and dew point. === */
void printBME280CalculatedData(Stream* client);
/* ==== END Prototypes ==== */

/* ==== Setup ==== */
void setup() {
  Serial.begin(SERIAL_BAUD);
  while(!Serial) {} // Wait
  while(!bme.begin()){
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }
}
/* ==== END Setup ==== */

/* ==== Loop ==== */
void loop() {
   printBME280Data(&Serial);
   delay(500);
}
/* ==== End Loop ==== */

/* ==== Functions ==== */
void printBME280Data(Stream* client){
  float temp(NAN), hum(NAN), pres(NAN);
   uint8_t pressureUnit(0);                                           // unit: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi
   bme.read(pres, temp, hum, metric, pressureUnit);                   // Parameters: (float& pressure, float& temp, float& humidity, bool celsius = false, uint8_t pressureUnit = 0x0)
  /* Alternatives to ReadData():
    float temp(bool celsius = false);
    float pres(uint8_t unit = 0);
    float hum();

    Keep in mind the temperature is used for humidity and
    pressure calculations. So it is more effcient to read
    temperature, humidity and pressure all together.
   */
  client->print("Temp: ");
  client->print(temp);
  client->print("°"+ String(metric ? 'C' :'F'));
  client->print("  \tHumedad: ");
  client->print(hum);
  client->print("%");
  client->print("  \tPresion: ");
  client->print(pres);
  client->print(" bares");
  float altitude = bme.alt(metric, pres + 250);
  float dewPoint = bme.dew(metric);
  client->print("  \tAltitud: ");
  client->print(altitude);
  client->print((metric ? "m" : "ft"));
  client->print("  \tPunto rocío: ");
  client->print(dewPoint);
  client->println("°"+ String(metric ? 'C' :'F'));
}
/* ==== END Functions ==== */
