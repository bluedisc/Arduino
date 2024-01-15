#include <Arduino.h>
#include <calipers.h>
#include <main.h>

Caliper::Caliper()
{
  _clk_pin = 0;
  _dat_pin = 0;
  _current = 0;
  _value = 0;
  _offset = 0;
  _invert = 0;
  _last = 0;
  _bit = 0;
  _reading = false;
  _changed = false;
}

Caliper::Caliper(uint16_t clk_pin, uint16_t dat_pin)
{
  _clk_pin = clk_pin;
  _dat_pin = dat_pin;
  _current = 0;
  _value = 0;
  _offset = 0;
  _invert = 0;
  _last = 0;
  _bit = 0;
  _reading = false;
  _changed = false;

  pinMode(_clk_pin, INPUT_PULLDOWN);
  pinMode(_dat_pin, INPUT_PULLDOWN);

}

void Caliper::Read21Bit()
{
  uint32_t local_system_millis = ticks;
  uint32_t delay = local_system_millis - _last;
  if (delay > 90)
  {
    _reading = true;
    _bit = 0;
    _value = 0;
  }
  else
  {
    if (_reading)
    {
      ++_bit;
      if (_bit > 0 && _bit <= 20)
      {
        if (digitalRead(_dat_pin) == 1)
        {
          _value |= 1 << (_bit - 1);
        }
      }
      if (_bit == 21)
      {
        if (digitalRead(_dat_pin) == 1)
        {
          _value = _value * -1 * _invert;
        }
        else
        {
          _value = _value * _invert;
        }
        if (_value != (_value - _offset))
        {
          _current = _value - _offset;
          _changed = true;
        }
        _reading = false;
      }
    }
  }
}