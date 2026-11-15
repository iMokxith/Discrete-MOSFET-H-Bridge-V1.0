/*
  BasicMotorTest.ino
  Demonstrates basic forward, reverse, speed ramping, and dynamic braking
  using the custom DiscreteHBridge library.

  Hardware:
  - Leg A input (Signal 1) -> Arduino Pin 9 (~PWM)
  - Leg B input (Signal 2) -> Arduino Pin 10 (~PWM)
  - GND                    -> Arduino GND
*/

#include <DiscreteHBridge.h>

// Initialize motor driver on PWM pins 9 (Leg A) and 10 (Leg B)
DiscreteHBridge motor(9, 10);

void setup() {
  Serial.begin(115200);

  // Initialize motor driver (starts safely in Brake mode)
  motor.begin();

  // Optional configurations:
  // motor.setMaxDutyCycle(90);    // Default is 90% (vital for bootstrap caps)
  // motor.setReverseDeadTime(50); // Pause in ms when switching directions
  // motor.invertDirection(false); // Swap if motor spins opposite desired forward

  Serial.println("=========================================");
  Serial.println("  Discrete H-Bridge Basic Test Started   ");
  Serial.println("=========================================");
  delay(1500);
}

void loop() {
  // 1. FORWARD at 50% Speed
  Serial.println(">>> 1. Forward at 50% Speed");
  motor.forward(50);
  delay(3000);

  // 2. FORWARD at 85% Speed
  Serial.println(">>> 2. Forward at 85% Speed");
  motor.forward(85);
  delay(3000);

  // 3. ACTIVE BRAKE
  Serial.println(">>> 3. Active Brake (Stop)");
  motor.brake();
  delay(2000);

  // 4. REVERSE at 50% Speed
  Serial.println(">>> 4. Reverse at 50% Speed");
  motor.reverse(50);
  delay(3000);

  // 5. REVERSE at 85% Speed
  Serial.println(">>> 5. Reverse at 85% Speed");
  motor.reverse(85);
  delay(3000);

  // 6. ACTIVE BRAKE
  Serial.println(">>> 6. Active Brake (Stop)");
  motor.brake();
  delay(2000);

  // 7. SMOOTH ACCELERATION RAMP
  Serial.println(">>> 7. Smoothly ramping from 0 to +80% over 1.5 seconds...");
  motor.rampTo(80, 1500);
  delay(2000);

  Serial.println(">>> 8. Smoothly ramping from +80% to -80% (Reverse) over 2.5 seconds...");
  motor.rampTo(-80, 2500);
  delay(2000);

  Serial.println(">>> 9. Ramping down to 0% (Stop)...");
  motor.rampTo(0, 1000);
  motor.brake();
  delay(3000);
}
