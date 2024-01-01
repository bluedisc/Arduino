#ifndef BUTTON_H
#define BUTTON_H

class Button {
public:
  Button();
  Button(uint16_t pin);
private:
  void push();
  uint16_t _pin;
  uint32_t _previous;
};

#endif
