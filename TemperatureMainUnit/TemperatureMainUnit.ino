#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

RF24 radio(8, 7); // CE, CSN

const byte address[6] = "00001";
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  lcd.begin(16,2);//Defining 16 columns and 2 rows of lcd display
  lcd.backlight();//To Power ON the back light

}
void loop() {
  lcd.setCursor(0,0);
  
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    String message = text;
    int delimiterIndex = message.indexOf("-");
    String temperature = message.substring(0,delimiterIndex);
    String humidity = message.substring(delimiterIndex + 1, message.length());

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: " + temperature + (char)223 + "C");
    lcd.setCursor(0, 1);
    lcd.print("RH: " + humidity + "%");
  
    
    // Serial.println(text);
    // lcd.print(text);
  } else {
    // lcd.print(" Radio not available ");
  }
}

