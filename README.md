# My-Projects
This Multifunction IC Tester is a versatile and cost-effective hardware tool designed using an ESP32 microcontroller. It allows users to test multiple types of electronic components, including digital ICs, linear ICs, resistors, capacitors, inductors, diodes, and to measure voltage and current. The device features a compact OLED display and intuitive interface, making it suitable for students, electronics enthusiasts, and professionals alike.

🛠️ Features
✅ Digital IC Testing
Test commonly used 74-series logic ICs (e.g., 7400, 7402, 7404, 7408, 7432, 7486, etc.) by simulating input logic levels and verifying output responses.

✅ Linear IC Testing
Validate the working of basic analog ICs like LM741 Op-Amp by checking output behavior under standard test configurations.

✅ Component Measurement

Resistors: Measure resistance up to 100kΩ

Capacitors: Measure capacitance (µF to nF range)

Inductors: Measure inductance in the mH/µH range

✅ Voltage Measurement
Measure DC voltages up to 15V using a voltage divider circuit (20kΩ and 10kΩ resistors).

✅ Current Measurement
Use the ACS712 current sensor module to measure current in the range of ±5A/±20A/±30A (based on the version used).

✅ OLED Display Interface
Display real-time results using a 128x32 I2C OLED for clean, minimal UI.

✅ User-Friendly GPIO Interface
Uses ESP32 GPIOs for input/output logic control, measurements, and driving test signals.

🧰 Components Used
Component	Description
ESP32 Dev Board	Core microcontroller
128x32 OLED Display	I2C interface, output display
ACS712 Current Module	Current sensing
Resistors	20kΩ, 10kΩ (for voltage divider)
Breadboard & Wires	Circuit setup
Test Sockets	For inserting ICs/components

⚙️ Functional Blocks
Logic Tester Engine: Drives logic combinations on digital IC inputs and verifies expected outputs.

Analog Test Engine: Checks op-amp ICs with known test cases.

Measurement Engine: Measures unknown R, C, L, V, and I values.

Display Driver: Updates the OLED with results in real-time.
