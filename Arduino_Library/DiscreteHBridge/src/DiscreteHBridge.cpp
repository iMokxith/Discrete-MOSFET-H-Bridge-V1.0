/**
 * @file DiscreteHBridge.cpp
 * @brief Implementation of DiscreteHBridge library
 * @author Mokshshith Pujari
 * @version 1.0.0
 */

#include "DiscreteHBridge.h"

DiscreteHBridge::DiscreteHBridge(uint8_t pinA, uint8_t pinB)
  : _pinA(pinA),
    _pinB(pinB),
    _maxDutyPercent(90),
    _deadTimeMs(40),
    _invertDir(false),
    _currentSpeed(0),
    _lastDirection(0) {
}

void DiscreteHBridge::begin() {
  pinMode(_pinA, OUTPUT);
  pinMode(_pinB, OUTPUT);
  brake();
}

void DiscreteHBridge::brake() {
  // Setting both pins HIGH turns both low-side MOSFETs (Q2 & Q4) ON.
  // This shorts both motor terminals to GND for safe, active dynamic braking.
  digitalWrite(_pinA, HIGH);
  digitalWrite(_pinB, HIGH);
  _currentSpeed = 0;
  _lastDirection = 0;
}

void DiscreteHBridge::stop() {
  brake();
}

uint8_t DiscreteHBridge::_calculatePwm(uint8_t speedPercent) {
  // Constrain speed to configured max duty to safeguard bootstrap capacitor refresh
  uint8_t safeSpeed = constrain(speedPercent, 0, _maxDutyPercent);

  // Inverted PWM logic:
  // 0% speed   -> PWM value 255 (pin held HIGH -> low-side ON, high-side OFF -> 0V)
  // 100% speed -> PWM value 0 (pulses LOW to turn high-side bootstrap ON)
  return map(safeSpeed, 0, 100, 255, 0);
}

void DiscreteHBridge::_applyDrive(int speedPercent) {
  int speed = constrain(speedPercent, -100, 100);

  if (_invertDir) {
    speed = -speed;
  }

  int8_t targetDirection = 0;
  if (speed > 0) targetDirection = 1;
  else if (speed < 0) targetDirection = -1;

  // Insert a safe dead-time pause when switching directions
  if (_lastDirection != 0 && targetDirection != 0 && _lastDirection != targetDirection) {
    brake();
    delay(_deadTimeMs);
  }

  if (targetDirection == 0) {
    brake();
  } else if (targetDirection > 0) {
    // Forward: Leg B low-side held ON (GND), Leg A modulated with inverted PWM
    digitalWrite(_pinB, HIGH);
    analogWrite(_pinA, _calculatePwm((uint8_t)speed));
    _currentSpeed = speedPercent;
    _lastDirection = 1;
  } else {
    // Reverse: Leg A low-side held ON (GND), Leg B modulated with inverted PWM
    digitalWrite(_pinA, HIGH);
    analogWrite(_pinB, _calculatePwm((uint8_t)(-speed)));
    _currentSpeed = speedPercent;
    _lastDirection = -1;
  }
}

void DiscreteHBridge::drive(int speedPercent) {
  _applyDrive(speedPercent);
}

void DiscreteHBridge::forward(uint8_t speedPercent) {
  drive((int)speedPercent);
}

void DiscreteHBridge::reverse(uint8_t speedPercent) {
  drive(-((int)speedPercent));
}

void DiscreteHBridge::rampTo(int targetSpeed, uint16_t rampTimeMs) {
  targetSpeed = constrain(targetSpeed, -100, 100);
  int startSpeed = _currentSpeed;

  if (startSpeed == targetSpeed) return;

  int delta = targetSpeed - startSpeed;
  int steps = abs(delta);
  if (steps == 0) return;

  uint16_t stepDelay = rampTimeMs / steps;
  if (stepDelay < 2) stepDelay = 2;

  int increment = (targetSpeed > startSpeed) ? 1 : -1;

  for (int s = startSpeed; s != targetSpeed; s += increment) {
    drive(s);
    delay(stepDelay);
  }
  drive(targetSpeed);
}

void DiscreteHBridge::setMaxDutyCycle(uint8_t maxPercent) {
  // Safe boundaries: 50% to 95%
  _maxDutyPercent = constrain(maxPercent, 50, 95);
}

void DiscreteHBridge::setReverseDeadTime(uint16_t deadTimeMs) {
  _deadTimeMs = deadTimeMs;
}

void DiscreteHBridge::invertDirection(bool invert) {
  _invertDir = invert;
}

int DiscreteHBridge::getCurrentSpeed() const {
  return _currentSpeed;
}

int8_t DiscreteHBridge::getDirection() const {
  return _lastDirection;
}
