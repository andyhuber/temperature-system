#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <dht.h>
#include <LowPower.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define VBATPIN A9
#define DHTPIN A5
#define DS18B20PIN A5
#define SERIALNUMBER "OT253Q12s"

RF24 radio(5, 6); // CE, CSN
const byte address[6] = "00001";
dht DHT;

OneWire oneWire(DS18B20PIN);
DallasTemperature d18b20Sensors(&oneWire);

int intervall = 480;
bool debug = false;
int sensorType = 2;  // 1:DHT 2:DS18B20

void setup() {
  if (debug) {
    Serial.begin(9600);
  }
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  if (sensorType == 2) {
    d18b20Sensors.begin();
  }

  delay(1500);
}

void loop() {
  
  float measuredBatVoltage = analogRead(VBATPIN);
  measuredBatVoltage *= 2;
  measuredBatVoltage *= 3.3;
  measuredBatVoltage /= 1024;
  
  String deviceValue = String(SERIALNUMBER);
  String batteryValue = String(measuredBatVoltage);
  String sensorValues;

  switch (sensorType) {
    case 1:
      sensorValues = getDHTValues();
      break;
    case 2:
      sensorValues = getDS18B20Values();
      break;
  }

  String commandToSend = deviceValue + ":" + sensorValues + ":" + batteryValue;
  int n = commandToSend.length();  
  char text[n+1];  
  strcpy(text, commandToSend.c_str()); 
  radio.write(&text, sizeof(text));
  
  if (debug) {
    LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);   
  } else {
    for(int i=0; i<intervall; i++) {
      LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);   
    }
  }
}

String getDHTValues() {
  DHT.read22(DHTPIN);
  return String(DHT.temperature) + "-" + String(DHT.humidity);
}

String getDS18B20Values() {
  d18b20Sensors.requestTemperatures(); 
  float celcius=d18b20Sensors.getTempCByIndex(0);
  return String(celcius);
}