const enum class State {
  start,
  loading,
  error,
  success
};

const enum class Event {
  SUBMIT,
  REJECT,
  RESOLVE
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

      break;

    case State::error:
      if (event == Event::SUBMIT) {
        return State::loading;
      }

      break;

    case State::success:
      break;

    default:
      break;
  }
}

void setup() {

}

void loop() {

}
