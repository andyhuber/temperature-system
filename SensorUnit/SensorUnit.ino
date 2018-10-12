#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <dht.h>

RF24 radio(8, 7); // CE, CSN
const byte address[6] = "00001";
dht DHT;
float prevTemp = 0;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  Serial.println("initialize .....");
  delay(1500);
}

void loop() {
  DHT.read22(A0);
  
  if (prevTemp != float(DHT.temperature)) {
    String temperature = String(DHT.temperature) + "-" + String(DHT.humidity);
    int n = temperature.length();  
    char text[n+1];  
    strcpy(text, temperature.c_str()); 
    radio.write(&text, sizeof(text));
    prevTemp = DHT.temperature;
    // Serial.println(String(DHT.temperature));
  }
  delay(5000);
}