enum class State {
  start,
  loading,
  error,
  success
};

enum class Event {
  SUBMIT,
  REJECT,
  RESOLVE,

  // Event::NONE acts as an event that is undefined or null.
  NONE
};

State screenFsm(State state, Event event) {
  switch(state) {
    case State::start:
      if (event == Event::SUBMIT) {
        return State::loading;
      }

      break;

    case State::loading:
      if (event == Event::RESOLVE) {
        return State::success;
      }

      if (event == Event::REJECT) {
        return State::error;
      }

      if (event == Event::SUBMIT) {
        return State::start;
      }

      break;

    case State::error:
      if (event == Event::SUBMIT) {
        return State::loading;
      }

      break;

    case State::success:
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

    if (serialCommand == "submit") {
      currentEvent = Event::SUBMIT;
    }

    if (serialCommand == "resolve") {
      currentEvent = Event::RESOLVE;
    }
	}

  currentState = screenFsm(currentState, currentEvent);

  // Reset current event or you'll end up in a loop in the FSM
  currentEvent = Event::NONE;

  delay(500);
}

void printCurrentStateAndEvent() {
  switch(currentEvent) {
    case Event::SUBMIT:
      Serial.println("Event::SUBMIT");
      break;
    case Event::REJECT:
      Serial.println("Event::REJECT");
      break;
    case Event::RESOLVE:
      Serial.println("Event::RESOLVE");
      break;
    default:
      break;
  }

  switch(currentState) {
    case State::start:
      Serial.println("State::start");
      break;
    case State::loading:
      Serial.println("State::loading");
      break;
    case State::error:
      Serial.println("State::error");
      break;
    case State::success:
      Serial.println("State::success");
      break;
    default:
      break;
  }
}
