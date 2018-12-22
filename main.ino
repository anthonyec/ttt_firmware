#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans18pt7b.h>
#include <math.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 128
#define OLED_RESET 4
#define TEXT_NUDGE_X 4
#define TEXT_NUDGE_Y 2
#define DEBUG false
#define ADD_BUTTON_PIN 6
#define UNDO_BUTTON_PIN 5

Adafruit_SSD1306 display(SCREEN_HEIGHT, SCREEN_WIDTH, &Wire, OLED_RESET);

int score[] = {0, 0};
int initalServe[] = {0, 1};

int currentHistoryIndex = 0;

// 99 x 2
uint8_t history[198] = {};

String serialCommand = "";
int addButtonState = 0;
int undoButtonState = 0;
int lastAddButtonState = 0;
int lastUndoButtonState = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(1);
  display.setTextWrap(false);
  display.clearDisplay();
  display.setFont(&FreeSans18pt7b);

  display.drawLine(0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2, WHITE);
  display.display();

  pinMode(ADD_BUTTON_PIN, INPUT);
  pinMode(UNDO_BUTTON_PIN, INPUT);
}

char* string2char(String command){
  if(command.length()!=0){
    char *p = const_cast<char*>(command.c_str());
    return p;
  }
}

void renderScoreNumber(int x, int y, int score, bool highlighted) {
  if (score > 9) {
    display.setTextSize(1);
  } else {
    display.setTextSize(2);
  }

  String scoreText = String(score);
  int16_t textX, textY;
  uint16_t textWidth, textHeight;

  if (highlighted) {
    display.fillRoundRect(x, y, SCREEN_WIDTH, SCREEN_HEIGHT / 2, 4, WHITE);
    display.setTextColor(BLACK);
  } else {
    display.setTextColor(WHITE);
  }

  display.getTextBounds(scoreText, x, y, &textX, &textY, &textWidth, &textHeight);

  if (DEBUG) {
    display.drawRect(x + (SCREEN_WIDTH / 2) - (textWidth / 2), y + (SCREEN_HEIGHT / 2 / 2) - (textHeight / 2), textWidth, textHeight, WHITE);
  }

  display.setCursor(x + (SCREEN_WIDTH / 2) - (textWidth / 2) - TEXT_NUDGE_X, y + (SCREEN_HEIGHT / 2 / 2) + (textHeight / 2) - TEXT_NUDGE_Y);
  display.print(scoreText);
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
  if (Serial.available() > 0) {
    serialCommand = Serial.readString();
    Serial.print(serialCommand);

    if (serialCommand == "add") {
      addScoreForPlayer(0);
    }

    if (serialCommand == "sub") {
      subtractScoreForPlayer(0);
    }

    serialCommand = "";
  }

  int addButtonReading = digitalRead(ADD_BUTTON_PIN);
  int undoButtonReading = digitalRead(UNDO_BUTTON_PIN);

  if (addButtonReading != lastAddButtonState) {
    lastDebounceTime = millis();
  }

  if (undoButtonReading != lastUndoButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (addButtonReading != addButtonState) {
      addButtonState = addButtonReading;

      if (addButtonState == HIGH) {
        addScoreForPlayer(1);
      }
    }
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (undoButtonReading != undoButtonState) {
      undoButtonState = undoButtonReading;

      if (undoButtonState == HIGH && currentHistoryIndex > 0) {
        undo();
      } else if (undoButtonState == HIGH && score[1] - 1 < 0) {
        flipIntialServe();
      }
    }
  }

  lastAddButtonState = addButtonReading;
  lastUndoButtonState = undoButtonReading;

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

void flipIntialServe() {
  int temp0 = initalServe[0];
  initalServe[0] = initalServe[1];
  initalServe[1] = temp0;
}
