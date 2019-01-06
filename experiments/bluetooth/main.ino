#include <SoftwareSerial.h>

#define BLUETOOTH_SPEED 9600

// Swap RX/TX connections on bluetooth chip
//   Pin 10 --> Bluetooth TX
//   Pin 11 --> Bluetooth RX
SoftwareSerial mySerial(10, 11); // RX, TX

const long int speeds[] = {
  300,
  1200,
  2400,
  4800,
  9600,
  19200,
  38400,
  57600,
  74880,
  115200,
  230400,
  250000
};

void setup() {
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);

  Serial.begin(9600);
  while (!Serial);

  mySerial.begin(38400);
  while (!mySerial);

  delay(2000);

  Serial.write("Start Setup\r\n");

  // checkForBT();

  // delay(2000);
  // mySerial.begin(9600);
  // delay(2000);

  // Serial.write("AT Setup\r\n");
  // mySerial.print("AT\r\n");
  // waitForResponse();

  // // mySerial.print("AT+ORGL\r\n");
  // // waitForResponse();

  // Serial.write("AT+UART=9600,0,0\r\n");
  // mySerial.print("AT+UART=9600,0,0\r\n");
  // waitForResponse();

  // Serial.write("ADDR?\r\n");
  // mySerial.print("AT+ADDR?\r\n");
  // waitForResponse();

  // Serial.write("End response\r\n");
}

void waitForResponse() {
  delay(2000);

  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }

  Serial.write("\n");
}

void checkForBT() {
  for(int i = 0; i<11; i++) {
    const long int speed = speeds[i];
    Serial.write("Check speed: ");
    Serial.print(speeds[i]);
    Serial.write(". Is ok? ");
    mySerial.begin(speed);
    mySerial.print("AT\r\n");
    waitForResponse();
    Serial.write("\r\n");
    mySerial.end();
  }
}

void loop() {
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }

  if (Serial.available()) {
    Serial.println(Serial.readString());
    mySerial.print(Serial.read());
  }
}
