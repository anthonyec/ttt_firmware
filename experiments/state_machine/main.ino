#include <AceButton.h>
using namespace ace_button;
#include <AdjustableButtonConfig.h>
#include <ButtonConfig.h>

enum class State;
enum class Event;

ButtonConfig pushButtonConfig;
AceButton plusButton(&pushButtonConfig);
AceButton undoButton(&pushButtonConfig);

void handlePushButtonEvent(AceButton*, uint8_t, uint8_t);

enum class State {
  start,
  playing,
  gameOver
};

enum class Event {
  PRESS_ADD,
  PRESS_UNDO,
  HOLD_UNDO,
  PLAYER_WON,
  UNDO_HISTORY_START,

  // Event::NONE acts as an event that is undefined or null.
  NONE
};

State gameFsm(State state, Event event) {
  switch(state) {
    case State::start:
      if (event == Event::PRESS_ADD) {
        addScoreToPlayer();
        return State::playing;
      }

      if (event == Event::PRESS_UNDO) {
        swapServe();
        return State::start;
      }

      if (event == Event::HOLD_UNDO) {
        resetGame();
        return State::start;
      }

      break;

    case State::playing:
      if (event == Event::PRESS_ADD) {
        addScoreToPlayer();
        return State::playing;
      }

      if (event == Event::PRESS_UNDO) {
        undo();
        return State::playing;
      }

      if (event == Event::HOLD_UNDO) {
        resetGame();
        return State::start;
      }

      if (event == Event::PLAYER_WON) {
        playSomeNiceCelebrationMusic();
        return State::gameOver;
      }

      if (event == Event::UNDO_HISTORY_START) {
        return State::start;
      }

      break;

    case State::gameOver:
      if (event == Event::PRESS_ADD) {
        resetGame();
        return State::start;
      }

      if (event == Event::PRESS_UNDO) {
        undo();
        return State::playing;
      }

      if (event == Event::HOLD_UNDO) {
        resetGame();
        return State::start;
      }

      break;

    default:
      return state;
  }

  return state;
}

State currentState = State::start;
Event currentEvent = Event::NONE;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.flush();
  Serial.println("Setup");

  pushButtonConfig.setEventHandler(handlePushButtonEvent);
  pushButtonConfig.setFeature(ButtonConfig::kFeatureLongPress);
  pushButtonConfig.setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);

  plusButton.init(6, LOW, 0);
  undoButton.init(5, LOW, 1);
}

void loop() {
  printCurrentStateAndEvent();

  if (Serial.available() > 0) {
		String serialCommand = Serial.readString();
		Serial.print("Serial->");
		Serial.println(serialCommand);

    if (serialCommand == "press add") {
      currentEvent = Event::PRESS_ADD;
    }

    if (serialCommand == "press undo") {
      currentEvent = Event::PRESS_UNDO;
    }

    if (serialCommand == "hold undo") {
      currentEvent = Event::HOLD_UNDO;
    }

    if (serialCommand == "player won") {
      currentEvent = Event::PLAYER_WON;
    }

    if (serialCommand == "undo history start") {
      currentEvent = Event::UNDO_HISTORY_START;
    }
	}

  plusButton.check();
  undoButton.check();

  currentState = gameFsm(currentState, currentEvent);

  // Reset current event or you'll end up in a loop in the FSM
  currentEvent = Event::NONE;
}

String lastPrint = "";

void printOnce(String str) {
  if (lastPrint != str) {
    lastPrint = str;
    Serial.println(str);
  }
}

void printCurrentStateAndEvent() {
  switch(currentEvent) {
    case Event::PRESS_ADD:
      printOnce("Event::PRESS_ADD");
      break;
    case Event::PRESS_UNDO:
      printOnce("Event::PRESS_UNDO");
      break;
    case Event::HOLD_UNDO:
      printOnce("Event::HOLD_UNDO");
      break;
    case Event::PLAYER_WON:
      printOnce("Event::PLAYER_WON");
      break;
    case Event::UNDO_HISTORY_START:
      printOnce("Event::UNDO_HISTORY_START");
      break;
    default:
      break;
  }

  switch(currentState) {
    case State::start:
      printOnce("State::start");
      break;
    case State::playing:
      printOnce("State::playing");
      break;
    case State::gameOver:
      printOnce("State::gameOver");
      break;
    default:
      break;
  }
}

void undo() {
  Serial.println("undo()");
}

void resetGame() {
  Serial.println("resetGame()");
}

void swapServe() {
  Serial.println("swapServe()");
}

void addScoreToPlayer() {
  Serial.println("addScoreToPlayer()");
}

void playSomeNiceCelebrationMusic() {
  Serial.println("playSomeNiceCelebrationMusic()");
}

void handlePushButtonEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  uint8_t id = button->getId();

  switch (eventType) {
    case AceButton::kEventPressed:
      switch(id) {
        case 0:
          Serial.println("handlePushButtonEvent()->kEventPressed->Event::PRESS_ADD");
          currentEvent = Event::PRESS_ADD;
          break;
        case 1:
          Serial.println("handlePushButtonEvent()->kEventPressed->Event::PRESS_UNDO");
          currentEvent = Event::PRESS_UNDO;
          break;
      }
      break;
    case AceButton::kEventLongPressed:
      currentEvent = Event::HOLD_UNDO;
      switch(id) {
        case 0:
          break;
        case 1:
          Serial.println("handlePushButtonEvent()->kEventPressed->Event::HOLD_UNDO");
          currentEvent = Event::HOLD_UNDO;
          break;
      }
      break;
  }
}
