/*
  Timebase callback
  This example shows how to configure HardwareTimer to execute a callback with some parameter at regular interval.
  Callback toggles pin.
  Once configured, there is only CPU load for callbacks executions.
*/

#if !defined(STM32_CORE_VERSION) || (STM32_CORE_VERSION  < 0x01090000)
#error "Due to API change, this sketch is compatible with STM32_CORE_VERSION  >= 0x02000000"
#endif

#include <U8x8lib.h>

#include <array>
#include <map>


// const uint8_t rowPins[] = {PA0, PA1, PA2, PA3};
std::array<uint32_t, 4> rowPins = {PA0, PA1, PA2, PA3};
std::array<uint32_t, 4> colPins = {PA4, PA5, PA6, PA7};
volatile uint8_t currentRowIndex = 0;
volatile uint8_t buttonState[] = {0,0};

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

std::size_t getPinIndex(uint32_t pinValue) {
    for (std::size_t i = 0; i < colPins.size(); i++) {
        if (colPins[i] == pinValue) {
            return i; // Return the index when the pin is found
        }
    }
    return colPins.size(); // Return a value outside the valid index range when not found
}


// Every second, print on serial MyData. And increment it.
void Update_IT_callback()
{
  // Serial.println(*data);
  // *data = *data + 1;
  digitalWrite(rowPins.at(currentRowIndex), LOW);
  currentRowIndex = (currentRowIndex + 1) % rowPins.size();
  digitalWrite(rowPins.at(currentRowIndex), HIGH);
  attachColumnInterrupts();
}

void configureRowPins() {
  for (std::size_t i = 0; i < rowPins.size(); i++) {
    pinMode(rowPins[i], OUTPUT);
  }
}

void configureColumnPins() {
  for (std::size_t i = 0; i < colPins.size(); i++) {
    pinMode(colPins[i], INPUT_PULLDOWN);
  }
}

void attachColumnInterrupts() {
  for (std::size_t col = 0; col < colPins.size(); col++) {
      attachInterrupt(digitalPinToInterrupt(colPins[col]), std::bind(handleButtonInterrupt, &colPins[col]), FALLING);
  }
}
void detachColumnInterrupts() {
  for (std::size_t col = 0; col < colPins.size(); col++) {
      detachInterrupt(digitalPinToInterrupt(colPins[col]));
  }
}
void setup()
{
//  Serial.begin(9600); 
  u8x8.begin();
  u8x8.setFont(u8x8_font_px437wyse700a_2x2_r);

#if defined(TIM1)
  TIM_TypeDef *Instance = TIM1;
#else
  TIM_TypeDef *Instance = TIM2;
#endif

  // Instantiate HardwareTimer object. Thanks to 'new' instanciation, HardwareTimer is not destructed when setup() function is finished.
  HardwareTimer *MyTim = new HardwareTimer(Instance);

  // configure pin in output mode
  configureRowPins();
  configureColumnPins();
  attachColumnInterrupts();

  MyTim->setOverflow(10, MICROSEC_FORMAT); // 100 Hz
  MyTim->attachInterrupt(Update_IT_callback);
  MyTim->resume();
  
  // u8x8.setBusClock(1);
  // u8x8.begin();
  // u8x8.setFont(u8x8_font_px437wyse700a_2x2_r);
  // u8x8.drawString(0, 20, "Starting");
}

void handleButtonInterrupt(uint32_t* data) {
  // Button interrupt callback
  // Handle button presses and releases here
  // Debounce by checking the button state after a short delay

  // Determine which column triggered the interrupt
  uint8_t col = getPinIndex(*data);
  uint8_t row = currentRowIndex;
  detachColumnInterrupts();
  buttonState[0] = row;
  buttonState[1] = col;
  updateDisplay();
}

void updateDisplay() {
  u8x8.drawString(0, 0, u8x8_u8toa(buttonState[0], 3));
  u8x8.drawString(0, 20, u8x8_u8toa(buttonState[1], 3));

  // u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);
  // u8x8.drawString(0, 2, "ABC defg");
}

void loop()
{
 /* Nothing to do all is done by hardware. Even no interrupt required. */
}