#include <Arduino.h>
#include <buttons.h>

Button::Button()
{
}

Button::Button(uint16_t pin)
{
  _pin = pin;
  pinMode(_pin, INPUT_PULLDOWN);
}

KeyPad::KeyPad()
{
}

KeyPad::KeyPad(std::vector<uint8_t> pulse, std::vector<uint8_t> poll)
{
  for (auto n: pulse)
  {
    pinMode(n, OUTPUT);
    _pulse.push_back(n);
  }
  for (auto n : poll)
  {
    _poll.push_back(new Button(n));
  }
}