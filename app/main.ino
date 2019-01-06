#include <math.h>
#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <AceButton.h>
using namespace ace_button;
#include <AdjustableButtonConfig.h>
#include <ButtonConfig.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans18pt7b.h>

#define DEBUG false
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 128
#define OLED_RESET 4
#define TEXT_NUDGE_X 4
#define TEXT_NUDGE_Y 2
#define BLUETOOTH_SPEED 38400
#define BLUETOOTH_COMMAND_PIN 9

enum class State;
enum class Event;

ButtonConfig pushButtonConfig;
AceButton plusButton(&pushButtonConfig);
AceButton undoButton(&pushButtonConfig);
Adafruit_SSD1306 display(SCREEN_HEIGHT, SCREEN_WIDTH, &Wire, OLED_RESET);
SoftwareSerial bluetooth(10, 11);

void handlePushButtonEvent(AceButton*, uint8_t, uint8_t);
bool hasEnteredState();
void dispatchEvent(Event);
void renderMainMenu();
void renderGameOverScreen();
void renderScoreScreen();
void renderScoreNumber(uint8_t, uint8_t, uint8_t, bool);
State gameFsm();

enum class State {
  mainMenu,
  start,
  playing,
  gameOver,
  none
};

enum class Event {
  RECEIVE_ADD,
  RECEIVE_UNDO,
  RECEIVE_RESET,
  PRESS_ADD,
  RELEASE_UNDO,
  HOLD_UNDO,
  PLAYER_WON,
  UNDO_HISTORY_START,
  NONE
};

bool isNewState = true;
State previousState = State::none;
State currentState = State::start;
Event currentEvent = Event::NONE;

uint8_t score[] = {0, 0};
uint8_t initalServe[] = {0, 1};
uint8_t currentHistoryIndex = 0;
uint8_t history[99] = {};

void setup() {
  Serial.begin(9600);
  while (!Serial);

  bluetooth.begin(BLUETOOTH_SPEED);
  while (!bluetooth);

  Serial.println(F("[SETUP START]"));

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(1);
  display.setTextWrap(false);
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  pushButtonConfig.setEventHandler(handlePushButtonEvent);
  pushButtonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  pushButtonConfig.setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);

  plusButton.init(6, LOW, 0);
  undoButton.init(5, LOW, 1);

  delay(2000);
}

void loop() {
  plusButton.check();
  undoButton.check();

  while (bluetooth.available() > 0) {
    char incoming = bluetooth.read();

    if (incoming == 97) {
      dispatchEvent(Event::RECEIVE_ADD);
    }

    if (incoming == 114) {
      dispatchEvent(Event::RECEIVE_RESET);
    }

    if (incoming == 117) {
      dispatchEvent(Event::RECEIVE_UNDO);
    }

    Serial.write(incoming);
  }

  State newState = gameFsm(currentState, currentEvent);

  if (newState != currentState) {
    isNewState = true;
  } else {
    isNewState = false;
  }

  currentState = newState;

  // Reset current event or you'll end up in a loop in the FSM
  currentEvent = Event::NONE;
}

State gameFsm(State state, Event event) {
  switch(state) {
    case State::mainMenu:
      if (event == Event::PRESS_ADD) {
        return State::start;
      }

      renderMainMenu();

      break;

    case State::start:
      if (hasEnteredState()) {
        Serial.println(F("Entered State::start"));
      }

      if (event == Event::RECEIVE_ADD) {
        addScoreForPlayer(0);
        return State::playing;
      }

      if (event == Event::PRESS_ADD) {
        addScoreForPlayer(1);
        bluetooth.print("a");
        return State::playing;
      }

      if (event == Event::RELEASE_UNDO) {
        bluetooth.print("u");
      }

      if (event == Event::RELEASE_UNDO || event == Event::RECEIVE_UNDO) {
        swapServe();
        return State::start;
      }

      if (event == Event::HOLD_UNDO) {
        return State::mainMenu;
      }

      renderScoreScreen();

      break;

    case State::playing:
      if (event == Event::RECEIVE_ADD) {
        addScoreForPlayer(0);
        return State::playing;
      }

      if (event == Event::PRESS_ADD) {
        addScoreForPlayer(1);
        bluetooth.print("a");
        return State::playing;
      }

      if (event == Event::RELEASE_UNDO) {
        bluetooth.print("u");
      }

      if (event == Event::RELEASE_UNDO || event == Event::RECEIVE_UNDO) {
        undo();
        return State::playing;
      }

      if (event == Event::HOLD_UNDO) {
        bluetooth.print("r");
      }

      if (event == Event::HOLD_UNDO || event == Event::RECEIVE_RESET) {
        resetGame();
        return State::start;
      }

      if ((score[0] >= 11 || score[1] >= 11) && abs(score[0] - score[1]) >= 2) {
        return State::gameOver;
      }

      if (currentHistoryIndex == 0) {
        return State::start;
      }

      renderScoreScreen();

      break;

    case State::gameOver:
      if (event == Event::PRESS_ADD) {
        resetGame();
        bluetooth.print("r");
        return State::start;
      }

      if (event == Event::RELEASE_UNDO) {
        bluetooth.print("u");
      }

      if (event == Event::RELEASE_UNDO || event == Event::RECEIVE_UNDO) {
        undo();
        return State::playing;
      }

      if (event == Event::HOLD_UNDO) {
        bluetooth.print("r");
      }

      if (event == Event::HOLD_UNDO || event == Event::RECEIVE_RESET) {
        resetGame();
        return State::start;
      }

      renderGameOverScreen();

      break;

    default:
      return state;
  }

  return state;
}

void dispatchEvent(Event event) {
  currentEvent = event;
}

bool hasEnteredState() {
  return isNewState;
}

void undo() {
  if (currentHistoryIndex - 1 >= 0) {
    uint8_t  playerIndexToSubtractScoreFrom = history[currentHistoryIndex - 1];
    subtractScoreForPlayer(playerIndexToSubtractScoreFrom);
    currentHistoryIndex -= 1;
  }
}

void resetGame() {
  score[0] = 0;
  score[1] = 0;
  currentHistoryIndex = 0;
}

void swapServe() {
  uint8_t temp0 = initalServe[0];
  initalServe[0] = initalServe[1];
  initalServe[1] = temp0;
}

void addScoreForPlayer(uint8_t playerIndex) {
  history[currentHistoryIndex] = playerIndex;

  score[playerIndex] += 1;
  currentHistoryIndex += 1;
}

void subtractScoreForPlayer(uint8_t playerIndex) {
  score[playerIndex] -= 1;
}

void handlePushButtonEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  uint8_t id = button->getId();

  switch (eventType) {
    case AceButton::kEventPressed:
      if (id == 0) {
        dispatchEvent(Event::PRESS_ADD);
      }

      break;

    case AceButton::kEventReleased:
      if (id == 1) {
        dispatchEvent(Event::RELEASE_UNDO);
      }

      break;
    case AceButton::kEventLongPressed:
      if (id == 1) {
        dispatchEvent(Event::HOLD_UNDO);
      }

      break;
    default:
      break;
  }
}

void renderMainMenu() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setFont();
  display.print(F("MENU"));

  display.fillRoundRect(0, 30, SCREEN_WIDTH, 32, 4, WHITE);

  display.display();
}

void renderGameOverScreen() {
  display.clearDisplay();
  display.setCursor(5, 50);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print(F("WIN!"));
  display.display();
}

void renderScoreScreen() {
  display.clearDisplay();
  display.setFont(&FreeSans18pt7b);

  uint8_t scoreDifference = abs(score[0] - score[1]);
  uint8_t scoreTotal = score[0] + score[1];
  uint8_t numberOfServes = (scoreDifference <= 1 && (scoreTotal >= 19)) ? 1 : 2;
  float serving = fmod(floor((scoreTotal) / numberOfServes), 2);

  renderScoreNumber(0, 0, score[0], serving == initalServe[0]);
  renderScoreNumber(0, SCREEN_HEIGHT/2, score[1], serving == initalServe[1]);

  display.display();
}

void renderScoreNumber(uint8_t x, uint8_t y, uint8_t score, bool highlighted) {
  uint8_t textSize = score > 9 ? 1 : 2;
  int16_t textX, textY;
  uint16_t textWidth, textHeight;
  char buffer[7];
  itoa(score, buffer, 10);


  if (highlighted) {
    display.fillRoundRect(x, y, SCREEN_WIDTH, SCREEN_HEIGHT / 2, 4, WHITE);
    display.setTextColor(BLACK);
  } else {
    display.setTextColor(WHITE);
  }

  display.setTextSize(textSize);

  // This is causing some glitchyness on the top-right of the screen.
  display.getTextBounds(buffer, x, y, &textX, &textY, &textWidth, &textHeight);
  display.setCursor(x + (SCREEN_WIDTH / 2) - (textWidth / 2) - TEXT_NUDGE_X, y + (SCREEN_HEIGHT / 2 / 2) + (textHeight / 2) - TEXT_NUDGE_Y);
  display.print(score);
}
