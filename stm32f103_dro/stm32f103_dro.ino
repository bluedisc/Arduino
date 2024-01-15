#include <vector>
#include <buttons.h>
#include <main.h>

volatile uint32_t ticks = 0;


TIM_TypeDef *Instance = TIM2;
HardwareTimer *MyTim = new HardwareTimer();

KeyPad* keypad = nullptr;

void setup() {
  std::vector<uint8_t> pulse = {PB5, PB6};
  std::vector<uint8_t> polls = {PB7, PB8, PB9};
  keypad = new KeyPad(pulse, polls);

  MyTim->setup(Instance);
  MyTim->setOverflow(1000, HERTZ_FORMAT); 
  MyTim->attachInterrupt(Update_IT_callback);
  MyTim->resume();  
}

void loop() {
  // put your main code here, to run repeatedly:

}

void Update_IT_callback()
{
  ++ticks;
}