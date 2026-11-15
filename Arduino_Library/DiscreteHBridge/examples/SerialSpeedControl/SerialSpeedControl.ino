/*
  SerialSpeedControl.ino
  Interactive serial monitor control for DiscreteHBridge.

  Open Serial Monitor at 115200 baud.
  Type any integer between -100 and +100 and press Enter:
    - Positive (e.g. 50): Forward at 50%
    - Negative (e.g. -75): Reverse at 75%
    - 0: Dynamic Brake / Stop
*/

#include <DiscreteHBridge.h>

DiscreteHBridge motor(9, 10);

void setup() {
  Serial.begin(115200);
  motor.begin();

  Serial.println("=========================================");
  Serial.println("  Discrete H-Bridge Serial Control Demo  ");
  Serial.println("=========================================");
  Serial.println("Enter target speed between -100 and +100:");
  Serial.println("  > Positive = Forward");
  Serial.println("  > Negative = Reverse");
  Serial.println("  > 0        = Dynamic Brake");
  Serial.println("-----------------------------------------");
}

void loop() {
  if (Serial.available() > 0) {
    int target = Serial.parseInt();

    // Consume any leftover newline/carriage returns
    while (Serial.available() > 0) {
      char c = Serial.peek();
      if (c == '\r' || c == '\n' || c == ' ') {
        Serial.read();
      } else {
        break;
      }
    }

    Serial.print("Command received: ");
    Serial.println(target);

    // Smoothly transition to commanded speed over 400ms
    motor.rampTo(target, 400);

    Serial.print("Current Speed: ");
    Serial.print(motor.getCurrentSpeed());
    Serial.print("% | Direction: ");
    int8_t dir = motor.getDirection();
    if (dir > 0) Serial.println("FORWARD");
    else if (dir < 0) Serial.println("REVERSE");
    else Serial.println("BRAKED / STOPPED");
  }
}
