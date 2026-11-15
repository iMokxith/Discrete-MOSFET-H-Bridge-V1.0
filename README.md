# Discrete All-N-Channel MOSFET H-Bridge Motor Driver

A complete open-source hardware and software project featuring an **all-N-channel discrete MOSFET H-Bridge motor driver** with bootstrap high-side drivers, designed from scratch without dedicated driver ICs. 

The project includes:
1. **KiCad Hardware Design**: Complete schematic and single-sided PCB layout optimized for DIY home chemical etching (toner transfer method).
2. **Fabrication Files**: 1:1 scale mirrored PDF plot files ready for laser printing on glossy paper.
3. **Arduino Library (`DiscreteHBridge`)**: A plug-and-play Arduino library with built-in bootstrap capacitor protection, dynamic braking, directional dead-time, and smooth acceleration ramping.

---

## ⚡ Specifications

| Parameter | Specification | Notes |
| :--- | :--- | :--- |
| **Operating Motor Voltage** | 12V – 24V DC | Limited by gate driver ratings & capacitor voltage |
| **Logic Control Voltage** | 5V (Arduino Uno / Nano / Mega) | 3.3V compatible via BC547 base resistors |
| **Control Interface** | 2-Wire Complementary (`IN_A`, `IN_B`) | Only 2 PWM pins required |
| **Power Switches** | 4x N-Channel MOSFETs (IRFZ44N / IRFP150N) | Ultra-low $R_{DS(on)}$ |
| **Gate Driver Stages** | 8x BJT Transistors (BC547 NPN + BC557 PNP) | Discrete push-pull stages |
| **Bootstrap Circuit** | 1N4148 diodes + 10µF–100µF capacitors | High-side gate charge pumping |
| **Flyback Protection** | 4x 1N4007 / Fast Diodes (UF4007 / FR307) | Clamps inductive spikes across motor |
| **PCB Layer** | Single-Layer (`F.Cu` / `B.Cu` compatible) | Optimized 1.5mm clearance for home etching |

---

## 📁 Repository Structure

```text
├── Hardware/                                # KiCad Project Files
│   ├── H_Bridge_motor_driver.kicad_pro      # KiCad project file
│   ├── H_Bridge_motor_driver.kicad_sch      # Complete schematic
│   ├── H_Bridge_motor_driver.kicad_pcb      # Single-layer routed PCB
│   └── Fabrication/                         # Etching transfer sheets
│       ├── H_Bridge_motor_driver-F_Cu.pdf   # 1:1 Mirrored copper layer
│       └── H_Bridge_motor_driver-Edge_Cuts.pdf # Board outline
│
├── Arduino_Library/                         # Arduino Driver
│   └── DiscreteHBridge/                     # Installable Arduino library
│       ├── library.properties
│       ├── keywords.txt
│       ├── src/
│       │   ├── DiscreteHBridge.h
│       │   └── DiscreteHBridge.cpp
│       └── examples/
│           ├── BasicMotorTest/
│           └── SerialSpeedControl/
│
├── .gitignore                               # Clean KiCad & build ignore rules
└── README.md
```

---

## 🔌 Hardware Circuit Architecture

### 1. High-Side Bootstrap Push-Pull Drive (Q1 & Q3)
High-side N-channel MOSFETs require their gate voltage to rise **~10V above the motor supply** to turn fully ON. 
- A bootstrap capacitor stores charge from the 12V supply via a 1N4148 diode when the half-bridge output is grounded.
- When the high-side MOSFET turns ON, the bootstrap capacitor floats up with the output, powering a discrete BC547/BC557 push-pull totem pole that rapidly charges and discharges the MOSFET gate capacitance.

### 2. Low-Side Pre-Inverted Push-Pull Drive (Q2 & Q4)
The low-side drivers feature a BC547 pre-inverter stage with a pull-up resistor to +12V. This ensures **Active-High** logic and provides cross-conduction shoot-through protection when paired with the complementary high-side drive.

### 3. Control Truth Table (2-Wire Interface)

| Mode | `IN_A` (Pin 9) | `IN_B` (Pin 10) | Motor State |
| :--- | :--- | :--- | :--- |
| **Dynamic Brake / Stop** | `HIGH` (5V) | `HIGH` (5V) | Both low-sides ON (Grounded) |
| **Forward** | **PWM** (Inverted) | `HIGH` (5V) | Leg B grounded, Leg A pulsed |
| **Reverse** | `HIGH` (5V) | **PWM** (Inverted) | Leg A grounded, Leg B pulsed |

---

## 💻 Arduino Library Usage

### Installation
Copy the `Arduino_Library/DiscreteHBridge` directory into your `Documents/Arduino/libraries/` folder, or zip it and import via **Sketch $\rightarrow$ Include Library $\rightarrow$ Add .ZIP Library**.

### Example Code

```cpp
#include <DiscreteHBridge.h>

// Leg A on Pin 9 (~PWM), Leg B on Pin 10 (~PWM)
DiscreteHBridge motor(9, 10);

void setup() {
  motor.begin(); // Initializes pins and starts in safe Brake mode
}

void loop() {
  // Drive forward at 75% speed
  motor.forward(75);
  delay(3000);

  // Active dynamic brake
  motor.brake();
  delay(1000);

  // Drive reverse at 75% speed
  motor.reverse(75);
  delay(3000);

  // Smoothly ramp down to 0% over 1 second
  motor.rampTo(0, 1000);
  motor.brake();
  delay(2000);
}
```

---

## 🛠️ DIY PCB Fabrication Notes

1. **Printing:** Print `Hardware/Fabrication/H_Bridge_motor_driver-F_Cu.pdf` at **100% scale (no fit to page)** on magazine paper or toner transfer paper using a laser printer.
2. **Transfer:** Iron onto single-sided copper clad board for ~3–5 minutes at high heat.
3. **Etching:** Etch in ferric chloride ($FeCl_3$) solution with agitation. The wide 1.5mm ground plane clearance prevents accidental shorts.
4. **Drilling:** KiCad drill marks are exported with "Small" pilot dimples to prevent drill bit wander when drilling by hand.
5. **Component Mounting:** All components are mounted on the copper side (`F.Cu`). Ensure 2mm–3mm standoffs on screw terminals and electrolytic capacitors for soldering iron access.

---

## 📄 License
This project is released under the **MIT License**. Feel free to use, modify, and build upon it for your own projects!
