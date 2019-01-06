#include <SoftwareSerial.h>

#define BLUETOOTH_SPEED 38400
#define RX_PIN 10
#define TX_PIN 11

SoftwareSerial bluetooth(RX_PIN, TX_PIN);

char myChar;

void setup() {
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);

  Serial.begin(9600);
  Serial.println("[SETUP BEGIN]");

  bluetooth.begin(BLUETOOTH_SPEED);

  delay(2000);

  bluetooth.print("AT\r\n");
}

void loop(){
  while(bluetooth.available()){
    myChar = bluetooth.read();
    Serial.print(myChar);
  }

  while(Serial.available()){
   myChar = Serial.read();
   bluetooth.print(myChar);
  }
}
