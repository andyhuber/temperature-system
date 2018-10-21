#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

RF24 radio(8, 7); // CE, CSN
long lastReceivedTimestamp = 0;
const byte address[6] = "00001";
bool notRespondingShowing = false;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  lcd.begin(16,2);
  lcd.backlight();
  lcd.print("Waiting...");
}

void loop() {
  
  if (Serial.available() > 0) {
    lcd.setCursor(0,0);
    String incomingTime = Serial.readString();
    lcd.clear();
    lcd.print(incomingTime);
  }
  
  if (radio.available()) {
    lcd.setCursor(0,0);
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text); // For the server

    char *splitChunkString = strtok(text, ":");
    char *groups[3] = {};
    int i = 0;
    while (splitChunkString != NULL)
    {
      groups[i++] = splitChunkString;
      splitChunkString = strtok (NULL, ":");
    }
    
    char *splitString = strtok(groups[1], "-");
    char *measurements[3] = {};
    int j = 0;
    while (splitString != NULL)
    {
      measurements[j++] = splitString;
      splitString = strtok (NULL, "-");
    }

    lcd.clear();

    String temperature = measurements[0];
    if (temperature != "") {
      lcd.setCursor(0, 0);
      lcd.print("Out: " + temperature + (char)223 + "C");
    }

    String humidity = measurements[1];
    if (humidity != "") {
      lcd.setCursor(0, 1);
      lcd.print("RH: " + humidity + "%");
    }
    
    if (groups != "") {
      String battery = groups[2];
      battery += "V";
      lcd.setCursor(11, 1);
      lcd.print(battery);
    }
    
    lastReceivedTimestamp = millis();
    notRespondingShowing = false;
  } else {
    long difference = millis() - lastReceivedTimestamp;
    if (difference > 600000) {
      if (!notRespondingShowing) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sensor not");
        lcd.setCursor(0, 1);
        lcd.print("responding");
        notRespondingShowing = true;
      }
    }
  }
}

