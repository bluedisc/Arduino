#ifndef KEYPAD_H
#define KEYPAD_H

#include <cstdint>
#include <vector>

#include <button.h>

class KeyPad {
public:
  KeyPad();
  KeyPad(std::vector<uint8_t> pulse, std::vector<uint8_t> poll,  TIM_TypeDef *timer);
  void setup(std::vector<uint8_t> pulse, std::vector<uint8_t> poll,  TIM_TypeDef *timer);
private:
  void poll();
  void set_current(Button *button);
  HardwareTimer *_timer;
  uint8_t _current_row;
  Button *_button;
  std::vector<uint8_t> _pulse;
  std::vector<Button*> _poll;
};

#endif
