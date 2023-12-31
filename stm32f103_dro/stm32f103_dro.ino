#include <vector>
#include <buttons.h>


KeyPad* keypad = nullptr;

void setup() {
  std::vector<uint8_t> pulse = {PB5, PB6};
  std::vector<uint8_t> polls = {PB7, PB8, PB9};
  keypad = new KeyPad(pulse, polls);
}

void loop() {
  // put your main code here, to run repeatedly:

}
