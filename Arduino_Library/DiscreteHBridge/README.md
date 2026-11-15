# DiscreteHBridge Arduino Library

An Arduino library designed specifically for **2-Wire All-N-Channel Discrete Bootstrap MOSFET H-Bridge Motor Drivers**.

Built for drivers using discrete push-pull stages (BC547 / BC557) and IRFZ44N / IRFP150N N-channel MOSFETs with bootstrap high-side drives.

---

## ⚡ Features

- **2-Wire Complementary Control**: Requires only 2 PWM pins (one for Leg A, one for Leg B).
- **Bootstrap Capacitor Protection**: Automatically caps the maximum duty cycle (default 90%) so high-side bootstrap capacitors never starve of charge at high speeds.
- **Directional Dead-Time Safety**: Automatically inserts a brief dynamic brake pause when reversing direction to prevent sudden mechanical stress and voltage spikes.
- **Active Dynamic Braking**: Pulls both pins `HIGH`, activating both low-side MOSFETs to short motor terminals to GND for an immediate stop.
- **Smooth Speed Ramping**: `rampTo()` method ramps motor speed smoothly to prevent huge inrush currents.
- **Software Direction Inversion**: Invert forward/reverse direction in software with `invertDirection(true)` without rewiring.

---

## 🔌 Hardware Connections

| PCB Pin | Arduino Pin | Description |
| :--- | :--- | :--- |
| **`IN_A` / Signal 1** | **Pin 9 (~PWM)** | Leg A control (Q1 High-Side + Q2 Low-Side) |
| **`IN_B` / Signal 2** | **Pin 10 (~PWM)** | Leg B control (Q3 High-Side + Q4 Low-Side) |
| **GND** | **GND** | Common ground between Arduino and 12V supply |

---

## 📊 Control Truth Table

| Mode | `IN_A` (Pin 9) | `IN_B` (Pin 10) | Motor Behavior |
| :--- | :--- | :--- | :--- |
| **Brake / Stop** | `HIGH` (5V) | `HIGH` (5V) | Both low-sides ON (Short to GND) |
| **Forward** | **PWM** (Inverted) | `HIGH` (5V) | Leg B grounded, Leg A pulsed |
| **Reverse** | `HIGH` (5V) | **PWM** (Inverted) | Leg A grounded, Leg B pulsed |

---

## 🚀 Quick Start

```cpp
#include <DiscreteHBridge.h>

// Initialize with Pin 9 (Leg A) and Pin 10 (Leg B)
DiscreteHBridge motor(9, 10);

void setup() {
  motor.begin(); // Initializes pins and starts in safe Brake mode
}

void loop() {
  motor.forward(70);   // Run forward at 70% speed
  delay(2000);

  motor.brake();       // Active dynamic brake
  delay(1000);

  motor.reverse(70);   // Run reverse at 70% speed
  delay(2000);

  motor.rampTo(0, 500); // Smoothly ramp to stop
  delay(2000);
}
```

---

## 📖 API Reference

### Initialization
- `DiscreteHBridge(uint8_t pinA, uint8_t pinB)`: Constructor defining control pins.
- `void begin()`: Configures pin modes and sets driver to initial brake state.

### Motion Control
- `void drive(int speedPercent)`: Signed speed from `-100` (full reverse) to `+100` (full forward). `0` activates brake.
- `void forward(uint8_t speedPercent)`: 0 to 100% forward.
- `void reverse(uint8_t speedPercent)`: 0 to 100% reverse.
- `void brake()` / `void stop()`: Immediate dynamic brake.
- `void rampTo(int targetSpeed, uint16_t rampTimeMs = 500)`: Smoothly transitions from current speed to target speed over a set time.

### Configuration
- `void setMaxDutyCycle(uint8_t maxPercent)`: Sets duty cycle limit (default: `90`). Prevents bootstrap starvation.
- `void setReverseDeadTime(uint16_t deadTimeMs)`: Sets pause duration when changing direction (default: `40ms`).
- `void invertDirection(bool invert)`: Inverts direction logic in software.

### Status Queries
- `int getCurrentSpeed()`: Returns current speed percentage (`-100` to `+100`).
- `int8_t getDirection()`: Returns `1` (Forward), `-1` (Reverse), or `0` (Brake).
