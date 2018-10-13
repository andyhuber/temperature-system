#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <dht.h>
#include <LowPower.h>

RF24 radio(8, 7); // CE, CSN
const byte address[6] = "00001";
dht DHT;
float prevTemp = 0;
int intervall = 480;
bool debug = true;

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  delay(1500);
}

void loop() {
  DHT.read22(5);

  if (prevTemp != float(DHT.temperature) || debug ) {
    String temperature = String(DHT.temperature) + "-" + String(DHT.humidity);
    int n = temperature.length();  
    char text[n+1];  
    strcpy(text, temperature.c_str()); 
    radio.write(&text, sizeof(text));
    prevTemp = DHT.temperature;
  }

  for(int i=0; i<intervall; i++) {
    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);   
  }
}