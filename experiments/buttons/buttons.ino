#include <AceButton.h>
#include <AdjustableButtonConfig.h>
#include <ButtonConfig.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans18pt7b.h>

using namespace ace_button;

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 128
#define OLED_RESET 4
#define UNDO_BUTTON_PIN 5
#define PLUS_BUTTON_PIN 6

Adafruit_SSD1306 display(SCREEN_HEIGHT, SCREEN_WIDTH, &Wire, OLED_RESET);

ButtonConfig plusButtonConfig;
ButtonConfig undoButtonConfig;

AceButton plusButton(&plusButtonConfig);
AceButton undoButton(&undoButtonConfig);

void handleUndoButtonEvent(AceButton*, uint8_t, uint8_t);
void handlePlusButtonEvent(AceButton*, uint8_t, uint8_t);

void setup() {
  Serial.begin(9600);
  while (! Serial);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(1);
  display.setTextWrap(false);
  display.clearDisplay();
  display.setFont(&FreeSans18pt7b);

  Serial.println("Setup");

  plusButtonConfig.setEventHandler(handlePlusButtonEvent);
  plusButtonConfig.setFeature(ButtonConfig::kFeatureLongPress);

  undoButtonConfig.setEventHandler(handleUndoButtonEvent);
  undoButtonConfig.setFeature(ButtonConfig::kFeatureLongPress);

  plusButton.init(PLUS_BUTTON_PIN, LOW);
  undoButton.init(UNDO_BUTTON_PIN, LOW);

  display.drawLine(0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2, WHITE);
  display.display();
}

void loop() {
  undoButton.check();
  plusButton.check();
}

void handleUndoButtonEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  switch (eventType) {
    case AceButton::kEventReleased:
      Serial.println("handleUndoButtonEvent->kEventReleased");
      break;

    case AceButton::kEventPressed:
      Serial.println("handleUndoButtonEvent->kEventPressed");
      break;

    case AceButton::kEventLongPressed:
      Serial.println("handleUndoButtonEvent->kEventLongPressed");
      break;
  }
}


void handlePlusButtonEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  switch (eventType) {
    case AceButton::kEventReleased:
      Serial.println("handlePlusButtonEvent->kEventReleased");
      break;

    case AceButton::kEventPressed:
      Serial.println("handlePlusButtonEvent->kEventPressed");
      break;

    case AceButton::kEventLongPressed:
      Serial.println("handlePlusButtonEvent->kEventLongPressed");
      break;
  }
}
