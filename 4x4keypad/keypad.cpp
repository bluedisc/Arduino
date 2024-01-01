#include <Arduino.h>
#include <keypad.h>

const uint32_t rowScanFreq = 100; // Hz

KeyPad::KeyPad()
{
  _timer = nullptr;
  _button = nullptr;
}

KeyPad::KeyPad(std::vector<uint8_t> pulse, std::vector<uint8_t> poll,  TIM_TypeDef *timer)
{
  _timer = nullptr;
  _button = nullptr;
  setup(pulse, poll, timer);
}

void KeyPad::setup(std::vector<uint8_t> pulse, std::vector<uint8_t> poll,  TIM_TypeDef *timer)
{
  _current_row = 0;
  _timer = new HardwareTimer(timer);
  _timer->setOverflow(rowScanFreq, HERTZ_FORMAT);
  _timer->attachInterrupt(std::bind(&KeyPad::poll, this));
  for (auto n: pulse)
  {
    pinMode(n, OUTPUT);
    _pulse.push_back(n);
  }
  for (auto n : poll)
  {
    Button *b = new Button(n);
    _poll.push_back(b);
  }
  _timer->resume();
}

void KeyPad::poll(void)
{
  digitalWrite(_pulse.at(_current_row), LOW);
  _current_row = (_current_row + 1) % _pulse.size();
  digitalWrite(_pulse.at(_current_row), HIGH);
}

void KeyPad::set_current(Button *button)
{
  _button = button;
}