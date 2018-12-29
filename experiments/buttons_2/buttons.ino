#include "ButtonEvent/ButtonEvent.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans18pt7b.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 128
#define OLED_RESET 4
#define ADD_BUTTON_PIN 6
#define UNDO_BUTTON_PIN 5

Adafruit_SSD1306 display(SCREEN_HEIGHT, SCREEN_WIDTH, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(1);
  display.setTextWrap(false);
  display.clearDisplay();
  display.setFont(&FreeSans18pt7b);

  ButtonEvent.addButton(UNDO_BUTTON_PIN,       //button pin
                        onDown,   //onDown event function
                        onUp,     //onUp event function
                        onHold,   //onHold event function
                        1000,     //hold time in milliseconds
                        onDouble, //double event function
                        200);     //double time interval

  Serial.println("Setup");

  display.drawLine(0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2, WHITE);
  display.display();
}

void onDown() {
  Serial.println("onDown");
}

void onUp() {
  Serial.println("onUp");
}

void onHold() {
  Serial.println("onHold");
}

void onDouble() {
  Serial.println("onDouble");
}

void loop() {
  ButtonEvent.loop();
}
