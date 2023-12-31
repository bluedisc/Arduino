#ifndef BUTTONS_H
#define BUTTONS_H

#include <cstdint>
#include <vector>

class Button {
public:
  Button(uint16_t pin);
private:
  uint16_t _pin;
};

class KeyPad {
public:
  KeyPad(std::vector<uint8_t> pulse, std::vector<uint8_t> poll);
private:
  std::vector<uint8_t> _pulse;
  std::vector<Button*> _poll;
};

#endif
