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

  currentState = gameFsm(currentState, currentEvent);

  // Reset current event or you'll end up in a loop in the FSM
  currentEvent = Event::NONE;

  delay(500);
}

void printCurrentStateAndEvent() {
  switch(currentEvent) {
    case Event::PRESS_ADD:
      Serial.println("Event::PRESS_ADD");
      break;
    case Event::PRESS_UNDO:
      Serial.println("Event::PRESS_UNDO");
      break;
    case Event::HOLD_UNDO:
      Serial.println("Event::HOLD_UNDO");
      break;
    case Event::PLAYER_WON:
      Serial.println("Event::PLAYER_WON");
      break;
    case Event::UNDO_HISTORY_START:
      Serial.println("Event::UNDO_HISTORY_START");
      break;
    default:
      break;
  }

  switch(currentState) {
    case State::start:
      Serial.println("State::start");
      break;
    case State::playing:
      Serial.println("State::playing");
      break;
    case State::gameOver:
      Serial.println("State::gameOver");
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
