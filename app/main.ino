#include <math.h>
#include <stdlib.h>
#include <SPI.h>
#include <Wire.h>

#include <AceButton.h>
using namespace ace_button;
#include <AdjustableButtonConfig.h>
#include <ButtonConfig.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans18pt7b.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 128
#define OLED_RESET 4
#define TEXT_NUDGE_X 4
#define TEXT_NUDGE_Y 2
#define DEBUG false
#define PLUS_BUTTON_PIN 6
#define UNDO_BUTTON_PIN 5

Adafruit_SSD1306 display(SCREEN_HEIGHT, SCREEN_WIDTH, &Wire, OLED_RESET);

int score[] = {0, 0};
int initalServe[] = {0, 1};
int currentHistoryIndex = 0;
uint8_t history[99] = {};

ButtonConfig pushButtonConfig;
AceButton plusButton(&pushButtonConfig);
AceButton undoButton(&pushButtonConfig);

void handlePlusButtonEvent(AceButton*, uint8_t, uint8_t);
void handleUndoButtonEvent(AceButton*, uint8_t, uint8_t);
void handlePushButtonEvent(AceButton*, uint8_t, uint8_t);

void setup() {
  Serial.begin(9600);
  while (! Serial);

  randomSeed(analogRead(0));

  Serial.println("Setup:start");

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(1);
  display.setTextWrap(false);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(random(10, 50), random(10, 150));
  display.print("OK.");
  display.display();

  display.setFont(&FreeSans18pt7b);

  delay(1000);

  pushButtonConfig.setEventHandler(handlePushButtonEvent);
  pushButtonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  pushButtonConfig.setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);

  plusButton.init(PLUS_BUTTON_PIN, LOW, 0);
  undoButton.init(UNDO_BUTTON_PIN, LOW, 1);

  Serial.println("Setup:end");
}

void renderScoreNumber(int x, int y, int score, bool highlighted) {
  if (score > 9) {
    display.setTextSize(1);
  } else {
    display.setTextSize(2);
  }

  char buffer[7];
  itoa(score, buffer, 10);

  int16_t textX, textY;
  uint16_t textWidth, textHeight;

  if (highlighted) {
    display.fillRoundRect(x, y, SCREEN_WIDTH, SCREEN_HEIGHT / 2, 4, WHITE);
    display.setTextColor(BLACK);
  } else {
    display.setTextColor(WHITE);
  }

  display.getTextBounds(buffer, x, y, &textX, &textY, &textWidth, &textHeight);

  if (DEBUG) {
    display.drawRect(x + (SCREEN_WIDTH / 2) - (textWidth / 2), y + (SCREEN_HEIGHT / 2 / 2) - (textHeight / 2), textWidth, textHeight, WHITE);
  }

  display.setCursor(x + (SCREEN_WIDTH / 2) - (textWidth / 2) - TEXT_NUDGE_X, y + (SCREEN_HEIGHT / 2 / 2) + (textHeight / 2) - TEXT_NUDGE_Y);
  display.print(score);
}

void renderScoreScreen() {
  display.clearDisplay();
  display.setFont(&FreeSans18pt7b);

  int totalScore = score[0] + score[1];
  double serving = fmod(floor((totalScore) / 2), 2);

  renderScoreNumber(0, 0, score[0], serving == initalServe[0]);
  renderScoreNumber(0, SCREEN_HEIGHT/2, score[1], serving == initalServe[1]);

  display.setTextColor(WHITE, BLACK);
  display.setCursor(5, 10);
  display.setFont();
  display.setTextSize(1);
  display.print(currentHistoryIndex);

  display.display();
}

void loop() {
  plusButton.check();
  undoButton.check();
  renderScoreScreen();
}

void addScoreForPlayer(int playerIndex) {
  history[currentHistoryIndex] = playerIndex;

  score[playerIndex] += 1;
  currentHistoryIndex += 1;
}

void subtractScoreForPlayer(int playerIndex) {
  score[playerIndex] -= 1;
}

void undo() {
  if (currentHistoryIndex - 1 >= 0) {
    int playerIndexToSubtractScoreFrom = history[currentHistoryIndex - 1];
    subtractScoreForPlayer(playerIndexToSubtractScoreFrom);
    currentHistoryIndex -= 1;
  }
}

void reset() {
  score[0] = 0;
  score[1] = 0;
  currentHistoryIndex = 0;
}

void flipIntialServe() {
  int temp0 = initalServe[0];
  initalServe[0] = initalServe[1];
  initalServe[1] = temp0;
}

void handlePushButtonEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  uint8_t id = button->getId();

  switch (eventType) {
    case AceButton::kEventReleased:
      if (id == 0) {
        addScoreForPlayer(1);
      }

      if (id == 1 && currentHistoryIndex > 0) {
        undo();
      } else if (score[1] - 1 < 0) {
        flipIntialServe();
      }

      break;
    case AceButton::kEventLongPressed:
      if (id == 1) {
        reset();
      }

      break;
  }
}
