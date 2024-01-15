#ifndef CALIPERS_H
#define CALIPERS_H

#include <cstdint>
#include <vector>

class Caliper {
public:
  Caliper();
  Caliper(uint16_t clk_pin, uint16_t dat_pin);
  void Read21Bit();
private:
  uint16_t _clk_pin;
  uint16_t _dat_pin;
  int16_t _current;
  int16_t _value;
  int16_t _offset;
  int16_t _invert;
  uint32_t _last;
  uint8_t _bit;
  bool _reading;
  bool _changed;

};

#endif
