#ifndef FSM_H
#define FSM_H

class Fsm {
  public:
    bool hasEnteredState();
    void dispatchEvent();
    void update();
}

#endif
