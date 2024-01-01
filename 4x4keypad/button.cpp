#include <Arduino.h>
#include <button.h>

const uint32_t rowScanFreq = 100; //Hz

Button::Button()
{
    _previous = 0;
}

Button::Button(uint16_t pin)
{
  _previous = 0;
  _pin = pin;
  pinMode(_pin, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(_pin), std::bind(&Button::push, this), RISING);
}

void Button::push()
{

}
