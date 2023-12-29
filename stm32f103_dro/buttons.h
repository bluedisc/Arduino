#ifndef BUTTONS_H
#define BUTTONS_H

#include <cstdint>
#include <vector>

class Button {
public:
  Button(uint32_t pin);
private:
  uint32_t _pin;
};

class KeyPad {
public:
  KeyPad(uint32_t pulse[], uint32_t pole[]);
private:
  std::vector<Button*> _pulse;
  std::vector<Button*> _pole;
};

#endif
