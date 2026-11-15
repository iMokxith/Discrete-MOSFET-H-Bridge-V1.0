/**
 * @file DiscreteHBridge.h
 * @brief Arduino Library for 2-Wire All-N-Channel Discrete Bootstrap MOSFET H-Bridge
 * @author Mokshith pujari
 * @version 1.0.0
 */

#ifndef DISCRETE_HBRIDGE_H
#define DISCRETE_HBRIDGE_H

#include <Arduino.h>

class DiscreteHBridge {
  public:
    /**
     * @brief Constructor
     * @param pinA Arduino PWM pin connected to Leg A (Controls Q1 & Q2)
     * @param pinB Arduino PWM pin connected to Leg B (Controls Q3 & Q4)
     */
    DiscreteHBridge(uint8_t pinA, uint8_t pinB);

    /**
     * @brief Initialize pins and set initial safe brake state.
     * Call this in Arduino setup().
     */
    void begin();

    /**
     * @brief Drive motor with signed speed (-100 to +100).
     * @param speedPercent Positive = Forward, Negative = Reverse, 0 = Brake.
     */
    void drive(int speedPercent);

    /**
     * @brief Drive motor Forward (0 to 100%).
     * @param speedPercent Speed percentage (0 to 100).
     */
    void forward(uint8_t speedPercent);

    /**
     * @brief Drive motor in Reverse (0 to 100%).
     * @param speedPercent Speed percentage (0 to 100).
     */
    void reverse(uint8_t speedPercent);

    /**
     * @brief Active Dynamic Braking.
     * Grounds both motor terminals through low-side MOSFETs (Q2 & Q4).
     */
    void brake();

    /**
     * @brief Alias for brake() - stops motor immediately.
     */
    void stop();

    /**
     * @brief Smoothly ramp to target speed to prevent inrush current spikes.
     * @param targetSpeed Signed target speed (-100 to +100).
     * @param rampTimeMs Total time for the ramp transition in milliseconds (default 500ms).
     */
    void rampTo(int targetSpeed, uint16_t rampTimeMs = 500);

    /**
     * @brief Set maximum allowable duty cycle (default: 90%).
     * Capping duty cycle is critical for high-side bootstrap capacitors to refresh.
     * @param maxPercent Maximum percentage (recommended 80 - 92%).
     */
    void setMaxDutyCycle(uint8_t maxPercent);

    /**
     * @brief Set pause duration when reversing direction (default: 40ms).
     * Prevents high mechanical torque shock and inductive kickback spikes.
     * @param deadTimeMs Pause time in milliseconds.
     */
    void setReverseDeadTime(uint16_t deadTimeMs);

    /**
     * @brief Invert forward and reverse directions in software.
     * @param invert Set true to reverse software directions without rewiring motor.
     */
    void invertDirection(bool invert);

    /**
     * @brief Get current speed (-100 to +100).
     */
    int getCurrentSpeed() const;

    /**
     * @brief Get current operating direction.
     * @return 1 for Forward, -1 for Reverse, 0 for Brake/Stopped.
     */
    int8_t getDirection() const;

  private:
    uint8_t _pinA;
    uint8_t _pinB;
    uint8_t _maxDutyPercent;
    uint16_t _deadTimeMs;
    bool _invertDir;
    int _currentSpeed;
    int8_t _lastDirection; // 1 = FWD, -1 = REV, 0 = STOP

    uint8_t _calculatePwm(uint8_t speedPercent);
    void _applyDrive(int speedPercent);
};

#endif // DISCRETE_HBRIDGE_H
