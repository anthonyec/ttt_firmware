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

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.write("Start Setup\r\n");

  // for(int i = 0; i<11; i++) {
  //   const long int speed = speeds[i];
  //   Serial.write("Check speed: ");
  //   Serial.print(speeds[i]);
  //   Serial.write(". Is ok? ");
  //   mySerial.begin(speed);
  //   mySerial.print("AT\r\n");
  //   waitForResponse();
  //   Serial.write("\r\n");
  //   mySerial.end();
  // }

  delay(2000);
  mySerial.begin(BLUETOOTH_SPEED);
  delay(2000);

  Serial.write("AT Setup\r\n");

  // // // Should respond with OK
  mySerial.print("AT\r\n");
  waitForResponse();

  // mySerial.print("AT+ORGL\r\n");
  // waitForResponse();

  mySerial.print("AT+ADDR?\r\n");
  // waitForResponse();

  Serial.write("End response\r\n");
}

void waitForResponse() {
  delay(1000);

  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }

  Serial.write("\n");
}

void loop() {

}
