#include "Arduino.h"
#include "stm32f1xx_hal.h"

#define ROW0_PIN PA0
#define ROW1_PIN PA1
#define ROW2_PIN PA2
#define ROW3_PIN PA3

#define COL0_PIN PA4
#define COL1_PIN PA5
#define COL2_PIN PA6
#define COL3_PIN PA7

volatile uint8_t buttonState[4][4];
volatile uint8_t lastButtonState[4][4];

// Timer setup for row control
TIM_HandleTypeDef htim2;
void configureTimer() {
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  __HAL_RCC_TIM2_CLK_ENABLE();

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7200 - 1; // Adjust this value to change the timer frequency
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10; // 10 ms
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim2);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
}

void configureRowPins() {
  pinMode(ROW0_PIN, OUTPUT);
  pinMode(ROW1_PIN, OUTPUT);
  pinMode(ROW2_PIN, OUTPUT);
  pinMode(ROW3_PIN, OUTPUT);
}

void setupColumnInterrupts() {
  attachInterrupt(digitalPinToInterrupt(COL0_PIN), handleButtonInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(COL1_PIN), handleButtonInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(COL2_PIN), handleButtonInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(COL3_PIN), handleButtonInterrupt, FALLING);
}

void setup() {
  HAL_Init();
  configureTimer();
  configureRowPins();

  // Initialize button states
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      buttonState[row][col] = HIGH; // Initialize to HIGH (released)
    }
  }

  setupColumnInterrupts();
  HAL_TIM_Base_Start(&htim2);
}

void loop() {
  // No code should be in the loop function as requested.
}

void handleButtonInterrupt() {
 // Button interrupt callback
  // Handle button presses and releases here
  // Debounce by checking the button state after a short delay

  // Determine which column triggered the interrupt
  uint8_t col = 0;
  if (digitalRead(COL0_PIN) == LOW) col = 0;
  else if (digitalRead(COL1_PIN) == LOW) col = 1;
  else if (digitalRead(COL2_PIN) == LOW) col = 2;
  else if (digitalRead(COL3_PIN) == LOW) col = 3;

  // Wait for debounce time
  delay(10); // Adjust this value as needed for your debouncing requirements

  // Read the button state again after debounce
  uint8_t state = digitalRead(COL0_PIN + col);

  // Determine the row that corresponds to the interrupt source
  uint8_t row = 0;
  if (state == LOW) {
    if (digitalRead(ROW0_PIN) == HIGH) row = 0;
    else if (digitalRead(ROW1_PIN) == HIGH) row = 1;
    else if (digitalRead(ROW2_PIN) == HIGH) row = 2;
    else if (digitalRead(ROW3_PIN) == HIGH) row = 3;
  }

  // Update button state and lastButtonState
  buttonState[row][col] = state;
  lastButtonState[row][col] = state;

  // Detach the interrupt for this column (button) until the next cycle
  detachInterrupt(digitalPinToInterrupt(COL0_PIN + col));

  // Reattach the interrupt for this column (button) after a delay (next cycle)
  delay(10); // Adjust this value as needed for your debouncing requirements
  attachInterrupt(digitalPinToInterrupt(COL0_PIN + col), handleButtonInterrupt, FALLING);}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  // Timer interrupt callback
  // Rotate through the rows here
  static uint8_t currentRow = 0;
  digitalWrite(ROW0_PIN + currentRow, LOW);
  currentRow = (currentRow + 1) % 4;
  digitalWrite(ROW0_PIN + currentRow, HIGH);
}
