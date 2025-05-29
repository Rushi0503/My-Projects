//display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ACS712.h>

#define CURRENT_PIN 12          
#define VCCc 5.0                 // Supply voltage to ACS712
#define ADC_RESOLUTION 4095     
#define SENSITIVITY 100.0       // Sensitivity in mV/A for 30A module

ACS712 ACS(12, VCCc, ADC_RESOLUTION, SENSITIVITY);

#define CONTINUITY_PIN 15     // GPIO15 for analog input
#define CONTINUITY_THRESHOLD 1.0  // Voltage threshold in volts

// OLED Display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Button Pins (Active High)
#define UP_BUTTON 23
#define DOWN_BUTTON 19
#define NEXT_BUTTON 18
// Variables

const int total_modes0 = 8;
int mode_index0 = 0;  // Start at first mode

const int total_modes1 = 3;
int mode_index1 = 0;  // Start at voltage modes

const int total_modes2 = 3;
int mode_index2 = 0; // Start at transistor modes

const int total_modes3 = 25;
int mode_index3 = 0; // Start at iC modes

// Modes Array
const char* master_modes[] = {
    "Voltage",
    "Current",
    "Resistance",
    "Capacitance",
    "Continuity",
    "IC Testing",
    "Transistor test",
    ""
};

const char* voltage_modes[] = {
  "5V",
  "9V",
  "15V"
};

const char* transistor_modes[] = {
  "NPN / N-MOS",
  "PNP / P-MOS",
  "TEST"
};

const char* ic_modes[] = {
  "Find_IC",
  "741",
  "7400",
  "7402",
  "7404",
  "7408",
  "7432",
  "7486",
  "7401",
  "7403",
  "7405",
  "7406",
  "7407",
  "7409",
  "7410",
  "7411",
  "7412",
  "7415",
  "7420",
  "7425",
  "7427",
  "7451",
  "7458",
  "74107",
  "555"
};
//display
const int pins[3] = {2, 25, 26};
//resistance
// Transistor base GPIOs
const int BASE1 = 32; // For 1kΩ
const int BASE2 = 33; // For 10kΩ
const int BASE3 = 13; // For 100kΩ
const int ADC_PIN = 4; // Analog input pin

// Known resistor values (Ω)
const float R1 = 973.0;
const float R2 = 9760.0;
const float R3 = 91100.0;

// Constants
const float ADC_REF = 3.3;      // ADC reference voltage
const int ADC_RES = 4095;       // 12-bit ADC resolution
const float V_IN = 2.7;         // Voltage after base-emitter drop
//resistance

//voltage
#define VOLTAGE_PIN 15  // GPIO15 as analog input

const float R01 = 21500.0; // 20kΩ
const float R02 = 8840.0; // 10kΩ
const float R03 = 39500.0; // 40k
const float R04 = 9770.0;
//voltage

//capacitance
  const int chargePin = 5;       // GPIO25 to charge capacitor
  const int dischargePin = 2;    // GPIO27 to discharge capacitor
  const int analogPin = 34;       // ADC input to measure voltage
  const float R = 1500.0;        // Resistor value (10kΩ)
  const int ADC_MAX = 4095;       // 12-bit resolution
//capacitance

#define L 12    // Left pin (GPIO 2)
#define M 14   // Middle pin (GPIO 25)
#define R 27   // Right pin (GPIO 26)

void buttons(){
  if (digitalRead(UP_BUTTON) == HIGH) {
        if (mode_index0 > 0) {
            mode_index0--;
            displayModes0();
        }
        delay(200);
    }

    // DOWN Button (Scroll Down)
    if (digitalRead(DOWN_BUTTON) == HIGH) {
        if (mode_index0 < total_modes0 - 1) {
            mode_index0++;
            displayModes0();
        }
        delay(200);
    }

    // NEXT Button (Select Mode)
    if (digitalRead(NEXT_BUTTON) == HIGH) {
        executeMode0(mode_index0);
        delay(1000);
        ESP.restart();  // Restart ESP32 after executing function
    }

}



// Function to Display Modes (3 at a time)
void displayModes0() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    int start = mode_index0 > 0 ? mode_index0 - 1 : 0;
    int end = start + 3;
    if (end > total_modes0) end = total_modes0;
    
    for (int i = start; i < end; i++) {
        display.setCursor(10, (i - start) * 10);
        if (i == mode_index0) {
            display.print("> "); // Highlight selected mode
        } else {
            display.print("  ");
        }
        display.print(master_modes[i]);
    }

    display.display();
}

void displayModes1() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    int start = mode_index1 > 0 ? mode_index1 - 1 : 0;
    int end = start + 3;
    if (end > total_modes1) end = total_modes1;
    
    for (int i = start; i < end; i++) {
        display.setCursor(10, (i - start) * 10);
        if (i == mode_index1) {
            display.print("> "); // Highlight selected mode
        } else {
            display.print("  ");
        }
        display.print(voltage_modes[i]);
    }

    display.display();
}

void displayModes2() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    int start = mode_index2 > 0 ? mode_index2 - 1 : 0;
    int end = start + 3;
    if (end > total_modes2) end = total_modes2;
    
    for (int i = start; i < end; i++) {
        display.setCursor(10, (i - start) * 10);
        if (i == mode_index2) {
            display.print("> "); // Highlight selected mode
        } else {
            display.print("  ");
        }
        display.print(transistor_modes[i]);
    }

    display.display();
}

void displayModes3() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    int start = mode_index3 > 0 ? mode_index3 - 1 : 0;
    int end = start + 3;
    if (end > total_modes3) end = total_modes3;
    
    for (int i = start; i < end; i++) {
        display.setCursor(10, (i - start) * 10);
        if (i == mode_index3) {
            display.print("> "); // Highlight selected mode
        } else {
            display.print("  ");
        }
        display.print(ic_modes[i]);
    }

    display.display();
}

// Function to Execute Selected Mode
void executeMode0(int mode) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(10, 10);
    display.print("Executing:");
    display.setCursor(10, 20);
    display.print(master_modes[mode]);

    display.display();
    Serial.print("Executing Mode: ");
    Serial.println(master_modes[mode]);

    // Call specific function based on selected mode
    switch (mode) {
        case 0: measureVoltage(); break;
        case 1: measureCurrent(); break;
        case 2: measureResistance(); break;
        case 3: measureCapacitance(); break;
        case 4: continuity(); break;
        case 5: testIC(); break;
        case 6: testTransistor(); break;
    }

    delay(2000);  // Pause before restart
}
void executeMode1(int mode) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(10, 10);
    display.print("Executing:");
    display.setCursor(10, 20);
    display.print(voltage_modes[mode]);

    display.display();
    Serial.print("Executing Mode: ");
    Serial.println(voltage_modes[mode]);

    // Call specific function based on selected mode
    switch (mode) {
        case 0: measureVoltage3(); break;
        case 1: measureVoltage9(); break;
        case 2: measureVoltage15(); break;
    }

    delay(2000);  // Pause before restart
}

void executeMode2(int mode) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(10, 10);
    display.print("Executing:");
    display.setCursor(10, 20);
    display.print(transistor_modes[mode]);

    display.display();
    Serial.print("Executing Mode: ");
    Serial.println(transistor_modes[mode]);

    // Call specific function based on selected mode
    switch (mode) {
        case 0: check_npn(); break;
        case 1: check_pnp(); break;
        case 2: TEST(); break;
    }

    delay(2000);  // Pause before restart
}

void executeMode3(int mode) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(10, 10);
    display.print("Executing:");
    display.setCursor(10, 20);
    display.print(ic_modes[mode]);

    display.display();
    Serial.print("Executing Mode: ");
    Serial.println(ic_modes[mode]);

    // Call specific function based on selected mode
    switch (mode) {
    case 0: Find_IC(); break;
    case 1: check_741(); break;
    case 2: check_7400(); break;
    case 3: check_7402(); break;
    case 4: check_7404(); break;
    case 5: check_7408(); break;
    case 6: check_7432(); break;
    case 7: check_7486(); break;
    case 8: check_7401(); break;
    case 9: check_7403(); break;
    case 10: check_7405(); break;
    case 11: check_7406(); break;
    case 12: check_7407(); break;
    case 13: check_7409(); break;
    case 14: check_7410(); break;
    case 15: check_7411(); break;
    case 16: check_7412(); break;
    case 17: check_7415(); break;
    case 18: check_7420(); break;
    case 19: check_7425(); break;
    case 20: check_7427(); break;
    case 21: check_7451(); break;
    case 22: check_7458(); break;
    case 23: check_74107(); break;
    case 24: check_555(); break;
}

    delay(2000);  // Pause before restart
}

void Find_IC() {
  display.clearDisplay();

  ///////////////////////////////////// 7400: NAND Gate /////////////////////////////////////
  const int inputA_7400 = 2;
  const int inputB_7400 = 25;
  const int outputY_7400 = 26;

  pinMode(inputA_7400, OUTPUT);
  pinMode(inputB_7400, OUTPUT);
  pinMode(outputY_7400, INPUT);

  bool testPass7400 = true;
  digitalWrite(inputA_7400, LOW); digitalWrite(inputB_7400, LOW); delay(10);
  if (digitalRead(outputY_7400) != HIGH) testPass7400 = false;

  digitalWrite(inputA_7400, LOW); digitalWrite(inputB_7400, HIGH); delay(10);
  if (digitalRead(outputY_7400) != HIGH) testPass7400 = false;

  digitalWrite(inputA_7400, HIGH); digitalWrite(inputB_7400, LOW); delay(10);
  if (digitalRead(outputY_7400) != HIGH) testPass7400 = false;

  digitalWrite(inputA_7400, HIGH); digitalWrite(inputB_7400, HIGH); delay(10);
  if (digitalRead(outputY_7400) != LOW) testPass7400 = false;

  ///////////////////////////////////// 7402: NOR Gate /////////////////////////////////////
  const int inputA_7402 = 26;
  const int inputB_7402 = 25;
  const int outputY_7402 = 2;

  pinMode(inputA_7402, OUTPUT);
  pinMode(inputB_7402, OUTPUT);
  pinMode(outputY_7402, INPUT);

  bool testPass7402 = true;
  digitalWrite(inputA_7402, LOW); digitalWrite(inputB_7402, LOW); delay(10);
  if (digitalRead(outputY_7402) != HIGH) testPass7402 = false;

  digitalWrite(inputA_7402, LOW); digitalWrite(inputB_7402, HIGH); delay(10);
  if (digitalRead(outputY_7402) != LOW) testPass7402 = false;

  digitalWrite(inputA_7402, HIGH); digitalWrite(inputB_7402, LOW); delay(10);
  if (digitalRead(outputY_7402) != LOW) testPass7402 = false;

  digitalWrite(inputA_7402, HIGH); digitalWrite(inputB_7402, HIGH); delay(10);
  if (digitalRead(outputY_7402) != LOW) testPass7402 = false;

  ///////////////////////////////////// 7404: Inverter (NOT) /////////////////////////////////////
  const int inputA_7404 = 2;
  const int outputY_7404 = 25;

  pinMode(inputA_7404, OUTPUT);
  pinMode(outputY_7404, INPUT);

  bool testPass7404 = true;
  digitalWrite(inputA_7404, LOW); delay(10);
  if (digitalRead(outputY_7404) != HIGH) testPass7404 = false;

  digitalWrite(inputA_7404, HIGH); delay(10);
  if (digitalRead(outputY_7404) != LOW) testPass7404 = false;

  ///////////////////////////////////// 7408: AND Gate /////////////////////////////////////
  const int inputA_7408 = 2;
  const int inputB_7408 = 25;
  const int outputY_7408 = 26;

  pinMode(inputA_7408, OUTPUT);
  pinMode(inputB_7408, OUTPUT);
  pinMode(outputY_7408, INPUT);

  bool testPass7408 = true;
  digitalWrite(inputA_7408, LOW); digitalWrite(inputB_7408, LOW); delay(10);
  if (digitalRead(outputY_7408) != LOW) testPass7408 = false;

  digitalWrite(inputA_7408, LOW); digitalWrite(inputB_7408, HIGH); delay(10);
  if (digitalRead(outputY_7408) != LOW) testPass7408 = false;

  digitalWrite(inputA_7408, HIGH); digitalWrite(inputB_7408, LOW); delay(10);
  if (digitalRead(outputY_7408) != LOW) testPass7408 = false;

  digitalWrite(inputA_7408, HIGH); digitalWrite(inputB_7408, HIGH); delay(10);
  if (digitalRead(outputY_7408) != HIGH) testPass7408 = false;

  ///////////////////////////////////// 7432: OR Gate /////////////////////////////////////
  const int inputA_7432 = 2;
  const int inputB_7432 = 25;
  const int outputY_7432 = 26;

  pinMode(inputA_7432, OUTPUT);
  pinMode(inputB_7432, OUTPUT);
  pinMode(outputY_7432, INPUT);

  bool testPass7432 = true;
  digitalWrite(inputA_7432, LOW); digitalWrite(inputB_7432, LOW); delay(10);
  if (digitalRead(outputY_7432) != LOW) testPass7432 = false;

  digitalWrite(inputA_7432, LOW); digitalWrite(inputB_7432, HIGH); delay(10);
  if (digitalRead(outputY_7432) != HIGH) testPass7432 = false;

  digitalWrite(inputA_7432, HIGH); digitalWrite(inputB_7432, LOW); delay(10);
  if (digitalRead(outputY_7432) != HIGH) testPass7432 = false;

  digitalWrite(inputA_7432, HIGH); digitalWrite(inputB_7432, HIGH); delay(10);
  if (digitalRead(outputY_7432) != HIGH) testPass7432 = false;

  ///////////////////////////////////// 7486: XOR Gate /////////////////////////////////////
  const int inputA_7486 = 2;
  const int inputB_7486 = 25;
  const int outputY_7486 = 26;

  pinMode(inputA_7486, OUTPUT);
  pinMode(inputB_7486, OUTPUT);
  pinMode(outputY_7486, INPUT);

  bool testPass7486 = true;
  digitalWrite(inputA_7486, LOW); digitalWrite(inputB_7486, LOW); delay(10);
  if (digitalRead(outputY_7486) != LOW) testPass7486 = false;

  digitalWrite(inputA_7486, LOW); digitalWrite(inputB_7486, HIGH); delay(10);
  if (digitalRead(outputY_7486) != HIGH) testPass7486 = false;

  digitalWrite(inputA_7486, HIGH); digitalWrite(inputB_7486, LOW); delay(10);
  if (digitalRead(outputY_7486) != HIGH) testPass7486 = false;

  digitalWrite(inputA_7486, HIGH); digitalWrite(inputB_7486, HIGH); delay(10);
  if (digitalRead(outputY_7486) != LOW) testPass7486 = false;

  ///////////////////////////////////// Result Display /////////////////////////////////////
  if (testPass7400) {
    display.println("7400 FOUND");
  }
  else if (testPass7402) {
    display.println("7402 FOUND");
  }
  else if (testPass7408) {
    display.println("7408 FOUND");
  }
  else if (testPass7432) {
    display.println("7432 FOUND");
  }
  else if (testPass7486) {
    display.println("7486 FOUND");
  }
  else if (testPass7404) {
    display.println("7404 FOUND");
  }
  else {
    display.println("IC Not Working");
  }
  display.display();
  delay(500);
}

//LINEAR IC
void check_555() {
  // GPIO Mappings
  const int TRIG = 2;
  const int OUT = 25;
  const int RESET = 26;
  const int CTRL = 32;   // We leave this floating or pull low (optional)
  const int THR = 33;
  const int DISCH = 13;
  const int GND = 27;

  pinMode(GND, OUTPUT);
  digitalWrite(GND, LOW);
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, HIGH);  // Keep 555 enabled

  pinMode(TRIG, INPUT);   // Inputs: Trigger, Threshold
  pinMode(THR, INPUT);
  pinMode(OUT, INPUT);    // Output pin of 555
  pinMode(DISCH, INPUT);  // Monitor discharge (open collector)
  pinMode(CTRL, INPUT);   // Optional input for control voltage

  int highCount = 0;
  int lowCount = 0;

  unsigned long start = millis();
  while (millis() - start < 3000) { // 3-second test window
    int val = digitalRead(OUT);
    if (val == HIGH) highCount++;
    else lowCount++;
    delay(10);
  }

  if (highCount > 10 && lowCount > 10) {
    display.print("555 WORKING");
  } else {
    display.print("555 FAULTY");
  }
  display.display();
  delay(500);
}


void check_741() {
  // GPIO Mappings
  const int VCC = 33;    // Pin 7
  const int IN_PLUS = 26;  // Pin 3 (non-inverting)
  const int IN_MINUS = 25; // Pin 2 (inverting)
  const int OUT = 32;      // Pin 6

  // Power the Op-Amp
  pinMode(VCC, OUTPUT);
  digitalWrite(VCC, HIGH);

  // Setup input test voltages (using internal pullups & pulldowns)
  pinMode(IN_PLUS, OUTPUT);
  pinMode(IN_MINUS, OUTPUT);
  pinMode(OUT, INPUT);

  // Test 1: IN+ > IN- → Output should be HIGH
  digitalWrite(IN_PLUS, HIGH);  // 3.3V
  digitalWrite(IN_MINUS, LOW);  // 0V
  delay(10);
  bool test1 = digitalRead(OUT) == HIGH;


  if (test1) {
    display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
    display.println("741 WORKING");
    display.display();
    Serial.println("741 WORKING");
  } else {
    display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
    display.println("741 FAULTY");
    display.display();
    Serial.println("741 not WORKING");
  }
}

//BASIC LOGIC GATE DIGITAL IC
void check_7400() {
  const int inputA = 2;  // IC pin 1
  const int inputB = 25;  // IC pin 2
  const int outputY = 26; // IC pin 3

  pinMode(inputA, OUTPUT);
  pinMode(inputB, OUTPUT);
  pinMode(outputY, INPUT);

  bool testPass = true;

  // NAND Truth Table: Y = !(A & B)
  // 0 0 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 0 1 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1 0 -> 1
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1 1 -> 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // Show result
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (testPass) {
    display.println("7400 WORKING");
    Serial.println("7400 WORKING");
  } else {
    display.println("7400 FAULTY");
    Serial.println("7400 not WORKING");
  }

  display.display(); // <- Important to update screen
}

void check_7402() {
  const int inputA = 26;  // IC pin 1
  const int inputB = 25;  // IC pin 2
  const int outputY = 2; // IC pin 3

  pinMode(inputA, OUTPUT);
  pinMode(inputB, OUTPUT);
  pinMode(outputY, INPUT);

  bool testPass = true;

  // NAND Truth Table: Y = !(A & B)
  // 0 0 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 0 1 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 1 0 -> 1
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 1 1 -> 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // Show result
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (testPass) {
    display.println("7402 WORKING");
    Serial.println("7402 WORKING");
  } else {
    display.println("7402 FAULTY");
    Serial.println("7402 not WORKING");
  }

  display.display(); // <- Important to update screen
}

void check_7404() {
  pinMode(2, OUTPUT); // IC Pin 1
  pinMode(25, INPUT);  // IC Pin 2

  digitalWrite(2, LOW);
  delay(10);
  bool output = digitalRead(25);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  if (output == HIGH) {
    display.println("7404 WORKING");
    Serial.println("7404 WORKING");
  } else {
    display.println("7404 FAULTY");
    Serial.println("7404 FAULTY");
  }
  display.display();
}

void check_7408() {
  const int inputA = 2;  // IC pin 1
  const int inputB = 25;  // IC pin 2
  const int outputY = 26; // IC pin 3

  pinMode(inputA, OUTPUT);
  pinMode(inputB, OUTPUT);
  pinMode(outputY, INPUT);

  bool testPass = true;

  // NAND Truth Table: Y = !(A & B)
  // 0 0 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 0 1 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 1 0 -> 1
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 1 1 -> 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // Show result
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (testPass) {
    display.println("7408 WORKING");
    Serial.println("7408 WORKING");
  } else {
    display.println("7408 FAULTY");
    Serial.println("7408 not WORKING");
  }

  display.display(); // <- Important to update screen
}

void check_7432() {
  const int inputA = 2;  // IC pin 1
  const int inputB = 25;  // IC pin 2
  const int outputY = 26; // IC pin 3

  pinMode(inputA, OUTPUT);
  pinMode(inputB, OUTPUT);
  pinMode(outputY, INPUT);

  bool testPass = true;

  // NAND Truth Table: Y = !(A & B)
  // 0 0 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 0 1 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1 0 -> 1
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1 1 -> 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // Show result
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (testPass) {
    display.println("7432 WORKING");
    Serial.println("7432 WORKING");
  } else {
    display.println("7432 FAULTY");
    Serial.println("7432 not WORKING");
  }

  display.display(); // <- Important to update screen
}

void check_7486() {
  const int inputA = 2;  // IC pin 1
  const int inputB = 25;  // IC pin 2
  const int outputY = 26; // IC pin 3

  pinMode(inputA, OUTPUT);
  pinMode(inputB, OUTPUT);
  pinMode(outputY, INPUT);

  bool testPass = true;

  // NAND Truth Table: Y = !(A & B)
  // 0 0 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 0 1 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1 0 -> 1
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1 1 -> 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // Show result
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (testPass) {
    display.println("7486 WORKING");
    Serial.println("7486 WORKING");
  } else {
    display.println("7486 FAULTY");
    Serial.println("7486 not WORKING");
  }

  display.display(); // <- Important to update screen
}


//EXTRA IC
void check_7401() {
  const int inputA = 26;  // IC pin 1
  const int inputB = 25;  // IC pin 2
  const int outputY = 2; // IC pin 3

  pinMode(inputA, OUTPUT);
  pinMode(inputB, OUTPUT);
  pinMode(outputY, INPUT);

  bool testPass = true;

  // NAND Truth Table: Y = !(A & B)
  // 0 0 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 0 1 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1 0 -> 1
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1 1 -> 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // Show result
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (testPass) {
    display.println("7401 WORKING");
    Serial.println("7401 WORKING");
  } else {
    display.println("7401 FAULTY");
    Serial.println("7401 not WORKING");
  }

  display.display(); // <- Important to update screen
}

void check_7403() {
  const int inputA = 2;  // IC pin 1
  const int inputB = 25;  // IC pin 2
  const int outputY = 26; // IC pin 3

  pinMode(inputA, OUTPUT);
  pinMode(inputB, OUTPUT);
  pinMode(outputY, INPUT);

  bool testPass = true;

  // NAND Truth Table: Y = !(A & B)
  // 0 0 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 0 1 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 1 0 -> 1
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 1 1 -> 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // Show result
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (testPass) {
    display.println("7403 WORKING");
    Serial.println("7403 WORKING");
  } else {
    display.println("7403 FAULTY");
    Serial.println("7403 not WORKING");
  }

  display.display(); // <- Important to update screen
}

void check_7405() {
  pinMode(2, OUTPUT); // IC Pin 1
  pinMode(25, INPUT);  // IC Pin 2

  digitalWrite(2, LOW);
  delay(10);
  bool output = digitalRead(25);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  if (output == HIGH) {
    display.println("7405 WORKING");
    Serial.println("7405 WORKING");
  } else {
    display.println("7405 FAULTY");
    Serial.println("7405 FAULTY");
  }
  display.display();
}

void check_7406() {
  pinMode(2, OUTPUT); // IC Pin 1
  pinMode(25, INPUT);  // IC Pin 2

  digitalWrite(2, LOW);
  delay(10);
  bool output = digitalRead(25);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  if (output == HIGH) {
    display.println("7406 WORKING");
    Serial.println("7406 WORKING");
  } else {
    display.println("7406 FAULTY");
    Serial.println("7406 FAULTY");
  }
  display.display();
}

void check_7407() {
  pinMode(2, OUTPUT); // IC Pin 1
  pinMode(25, INPUT);  // IC Pin 2

  digitalWrite(2, LOW);
  delay(10);
  bool output = digitalRead(25);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  if (output == LOW) {
    display.println("7407 WORKING");
    Serial.println("7407 WORKING");
  } else {
    display.println("7407 FAULTY");
    Serial.println("7407 FAULTY");
  }
  display.display();
}

void check_7409() {
  const int inputA = 2;  // IC pin 1
  const int inputB = 25;  // IC pin 2
  const int outputY = 26; // IC pin 3

  pinMode(inputA, OUTPUT);
  pinMode(inputB, OUTPUT);
  pinMode(outputY, INPUT);

  bool testPass = true;

  // NAND Truth Table: Y = !(A & B)
  // 0 0 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 0 1 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 1 0 -> 1
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 1 1 -> 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // Show result
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (testPass) {
    display.println("7409 WORKING");
    Serial.println("7409 WORKING");
  } else {
    display.println("7409 FAULTY");
    Serial.println("7409 not WORKING");
  }

  display.display(); // <- Important to update screen
}

void check_7410() {
  const int inputA = 2;    // IC pin 1
  const int inputB = 25;   // IC pin 2
  const int inputC = 27;   // IC pin 13
  const int outputY = 26;  // IC pin 12

  pinMode(inputA, OUTPUT);
  pinMode(inputB, OUTPUT);
  pinMode(inputC, OUTPUT);
  pinMode(outputY, INPUT);

  bool testPass = true;

  // All 8 input combinations for 3-input NAND
  // Format: A, B, C => Expected Output

  // 0 0 0 => 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 0 0 1 => 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 0 1 0 => 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 0 1 1 => 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1 0 0 => 1
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1 0 1 => 1
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1 1 0 => 1
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1 1 1 => 0 (Only this input gives LOW output)
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // Show result
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (testPass) {
    display.println("7410 WORKING");
    Serial.println("7410 WORKING");
  } else {
    display.println("7410 FAULTY");
    Serial.println("7410 FAULTY");
  }

  display.display(); // <- Important to update screen
}

void check_7411() {
  const int inputA = 2;    // IC pin 1
  const int inputB = 25;   // IC pin 2
  const int inputC = 27;   // IC pin 13
  const int outputY = 26;  // IC pin 12

  pinMode(inputA, OUTPUT);
  pinMode(inputB, OUTPUT);
  pinMode(inputC, OUTPUT);
  pinMode(outputY, INPUT);

  bool testPass = true;

  // All 8 input combinations for 3-input AND
  // Format: A, B, C => Expected Output

  // 0 0 0 => 0
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 0 0 1 => 0
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 0 1 0 => 0
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 0 1 1 => 0
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 1 0 0 => 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 1 0 1 => 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 1 1 0 => 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 1 1 1 => 1
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // Show result
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (testPass) {
    display.println("7411 WORKING");
    Serial.println("7411 WORKING");
  } else {
    display.println("7411 FAULTY");
    Serial.println("7411 FAULTY");
  }

  display.display(); // <- Important to update screen
}

void check_7412() {
  const int inputA = 2;    // IC pin 1
  const int inputB = 25;   // IC pin 2
  const int inputC = 27;   // IC pin 13
  const int outputY = 26;  // IC pin 12

  pinMode(inputA, OUTPUT);
  pinMode(inputB, OUTPUT);
  pinMode(inputC, OUTPUT);
  pinMode(outputY, INPUT);

  bool testPass = true;

  // All 8 input combinations for 3-input NAND
  // Format: A, B, C => Expected Output

  // 0 0 0 => 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 0 0 1 => 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 0 1 0 => 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 0 1 1 => 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1 0 0 => 1
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1 0 1 => 1
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1 1 0 => 1
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1 1 1 => 0 (Only this input gives LOW output)
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // Show result
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (testPass) {
    display.println("7412 WORKING");
    Serial.println("7412 WORKING");
  } else {
    display.println("7412 FAULTY");
    Serial.println("7412 FAULTY");
  }

  display.display(); // <- Important to update screen
}

void check_7415() {
  const int inputA = 2;    // IC pin 1
  const int inputB = 25;   // IC pin 2
  const int inputC = 27;   // IC pin 13
  const int outputY = 26;  // IC pin 12

  pinMode(inputA, OUTPUT);
  pinMode(inputB, OUTPUT);
  pinMode(inputC, OUTPUT);
  pinMode(outputY, INPUT);

  bool testPass = true;

  // All 8 input combinations for 3-input AND
  // Format: A, B, C => Expected Output

  // 0 0 0 => 0
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 0 0 1 => 0
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 0 1 0 => 0
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 0 1 1 => 0
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 1 0 0 => 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 1 0 1 => 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 1 1 0 => 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 1 1 1 => 1
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // Show result
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (testPass) {
    display.println("7415 WORKING");
    Serial.println("7415 WORKING");
  } else {
    display.println("7415 FAULTY");
    Serial.println("7415 FAULTY");
  }

  display.display(); // <- Important to update screen
}

void check_7420() {
  const int inputA = 12;  // Pin 1
  const int inputB = 14;  // Pin 2
  const int inputC = 2;  // Pin 3
  const int inputD = 25;   // Pin 4
  const int outputY = 26; // Pin 6

  pinMode(inputA, OUTPUT);
  pinMode(inputB, OUTPUT);
  pinMode(inputC, OUTPUT);
  pinMode(inputD, OUTPUT);
  pinMode(outputY, INPUT);

  bool testPass = true;

  // NAND Truth Table for 4 inputs: Y = !(A & B & C & D)

  // 0000 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, LOW);
  digitalWrite(inputD, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 0001 -> 1
  digitalWrite(inputD, HIGH);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 0111 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, HIGH);
  digitalWrite(inputD, HIGH);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1111 -> 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, HIGH);
  digitalWrite(inputD, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // Show result
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (testPass) {
    display.println("7420 WORKING");
    Serial.println("7420 WORKING");
  } else {
    display.println("7420 FAULTY");
    Serial.println("7420 FAULTY");
  }

  display.display();
}

void check_7425() {
  const int inputA = 12;  // IC pin 1
  const int inputB = 14;  // IC pin 2
  const int inputC = 2;  // IC pin 3
  const int inputD = 25;   // IC pin 4
  const int outputY = 26; // IC pin 6

  pinMode(inputA, OUTPUT);
  pinMode(inputB, OUTPUT);
  pinMode(inputC, OUTPUT);
  pinMode(inputD, OUTPUT);
  pinMode(outputY, INPUT);  // Use pull-down if needed

  bool testPass = true;

  // Test Case 1: 0 0 0 0 -> Output should be 1 (HIGH)
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, LOW);
  digitalWrite(inputD, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // Test Case 2: 1 0 0 0 -> Output should be 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, LOW);
  digitalWrite(inputD, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // Test Case 3: 0 1 0 0 -> Output should be 0
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, LOW);
  digitalWrite(inputD, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // Test Case 4: 1 1 1 1 -> Output should be 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, HIGH);
  digitalWrite(inputD, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // Display result
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (testPass) {
    display.println("7425 WORKING");
    Serial.println("7425 WORKING");
  } else {
    display.println("7425 FAULTY");
    Serial.println("7425 FAULTY");
  }

  display.display();
}

void check_7427() {
  const int inputA = 27;  // IC pin 1
  const int inputB = 2;  // IC pin 2
  const int inputC = 25;  // IC pin 3
  const int outputY = 26; // IC pin 6

  pinMode(inputA, OUTPUT);
  pinMode(inputB, OUTPUT);
  pinMode(inputC, OUTPUT);
  pinMode(outputY, INPUT);

  bool testPass = true;

  // NOR logic: Y = !(A + B + C)

  // 0 0 0 -> 1
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != HIGH) testPass = false;

  // 1 0 0 -> 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 0 1 0 -> 0
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, LOW);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 0 0 1 -> 0
  digitalWrite(inputA, LOW);
  digitalWrite(inputB, LOW);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // 1 1 1 -> 0
  digitalWrite(inputA, HIGH);
  digitalWrite(inputB, HIGH);
  digitalWrite(inputC, HIGH);
  delay(10);
  if (digitalRead(outputY) != LOW) testPass = false;

  // Show result
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (testPass) {
    display.println("7427 WORKING");
    Serial.println("7427 WORKING");
  } else {
    display.println("7427 FAULTY");
    Serial.println("7427 FAULTY");
  }

  display.display();
}

void check_7451() {
  const int A = 14;  // IC pin 1
  const int B = 27;  // IC pin 2
  const int C = 2;  // IC pin 4
  const int D = 25;  // IC pin 5
  const int Y = 26;  // IC pin 6

  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(Y, INPUT);

  bool testPass = true;

  struct TestVector {
    bool a, b, c, d;
    bool expectedY;
  };

  TestVector tests[] = {
    {0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0},
    {0, 0, 1, 1, 0},
    {1, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {0, 0, 0, 1, 1},
  };

  for (auto &t : tests) {
    digitalWrite(A, t.a);
    digitalWrite(B, t.b);
    digitalWrite(C, t.c);
    digitalWrite(D, t.d);
    delay(10);
    if (digitalRead(Y) != t.expectedY) testPass = false;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  if (testPass) {
    display.println("7451 WORKING");
    Serial.println("7451 WORKING");
  } else {
    display.println("7451 FAULTY");
    Serial.println("7451 FAULTY");
  }
  display.display();
}

void check_7458() {
  const int A = 14;  // IC pin 1
  const int B = 27;  // IC pin 2
  const int C = 2;  // IC pin 4
  const int D = 25;  // IC pin 5
  const int Y = 26;  // IC pin 6

  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(Y, INPUT);

  bool testPass = true;

  struct TestVector {
    bool a, b, c, d;
    bool expectedY;
  };

  TestVector tests[] = {
    {0, 0, 0, 0, 0},
    {1, 1, 0, 0, 1},
    {0, 0, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0},
  };

  for (auto &t : tests) {
    digitalWrite(A, t.a);
    digitalWrite(B, t.b);
    digitalWrite(C, t.c);
    digitalWrite(D, t.d);
    delay(10);
    if (digitalRead(Y) != t.expectedY) testPass = false;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  if (testPass) {
    display.println("7458 WORKING");
    Serial.println("7458 WORKING");
  } else {
    display.println("7458 FAULTY");
    Serial.println("7458 FAULTY");
  }
  display.display();
}

void check_74107() {
  const int J = 12;     // Pin 1
  const int K = 13;     // Pin 4
  const int CLK = 32;   // Pin 12
  const int RST = 33;   // Pin 13
  const int Q = 25;     // Pin 2

  pinMode(J, OUTPUT);
  pinMode(K, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(Q, INPUT);

  bool testPass = true;

  // Apply async reset
  digitalWrite(RST, LOW); delay(10);  // Reset active
  digitalWrite(RST, HIGH);            // Release reset
  delay(10);

  // Test Set: J=1, K=0
  digitalWrite(J, HIGH);
  digitalWrite(K, LOW);
  digitalWrite(CLK, HIGH); delay(5);  // Ensure high before falling edge
  digitalWrite(CLK, LOW); delay(10);
  if (digitalRead(Q) != HIGH) testPass = false;

  // Test Reset: J=0, K=1
  digitalWrite(J, LOW);
  digitalWrite(K, HIGH);
  digitalWrite(CLK, HIGH); delay(5);
  digitalWrite(CLK, LOW); delay(10);
  if (digitalRead(Q) != LOW) testPass = false;

  // Test Toggle: J=1, K=1
  digitalWrite(J, HIGH);
  digitalWrite(K, HIGH);
  digitalWrite(CLK, HIGH); delay(5);
  digitalWrite(CLK, LOW); delay(10);
  bool firstToggle = digitalRead(Q);
  digitalWrite(CLK, HIGH); delay(5);
  digitalWrite(CLK, LOW); delay(10);
  bool secondToggle = digitalRead(Q);
  if (firstToggle == secondToggle) testPass = false;

  // Display Result
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  if (testPass) {
    display.println("74107 WORKING");
    Serial.println("74107 WORKING");
  } else {
    display.println("74107 FAULTY");
    Serial.println("74107 FAULTY");
  }
  display.display();
}



void check_npn(){


  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("NPN Transistor Test");

  bool detected = false;

  // ----- Test 1: Collector = Left (L), Emitter = Right (R) -----
  pinMode(L, OUTPUT); digitalWrite(L, HIGH); // Collector = HIGH
  pinMode(R, INPUT);
  pinMode(M, OUTPUT); digitalWrite(M, HIGH); // Base = HIGH
  delay(10);
  bool voltage1 = digitalRead(R);  // Should be LOW if current flows

  pinMode(L, OUTPUT); digitalWrite(L, HIGH); // Collector = HIGH
  pinMode(R, INPUT);
  pinMode(M, OUTPUT); digitalWrite(M, LOW); // Base = HIGH
  delay(10);
  bool voltage01 = digitalRead(R);  // Should be LOW if current flows

  if (voltage1 == true && voltage01 == false) {
    display.println("NPN Transistor:");
    display.println("C = Left");
    display.println("E = Right");
    detected = true;
  }

  // ----- Test 2: Collector = Right (R), Emitter = Left (L) -----
  pinMode(L, INPUT); 
  pinMode(R, OUTPUT); digitalWrite(R, HIGH); // Collector = HIGH
  pinMode(M, OUTPUT); digitalWrite(M, HIGH); // Base = HIGH
  delay(10);
  bool voltage2 = digitalRead(L);  // Should be LOW if current flows

  pinMode(L, INPUT); 
  pinMode(R, OUTPUT); digitalWrite(R, HIGH); // Collector = HIGH
  pinMode(M, OUTPUT); digitalWrite(M, LOW); // Base = HIGH
  delay(10);
  bool voltage02 = digitalRead(L);  // Should be LOW if current flows

  if (!detected && voltage2 == true && voltage02 == false) {
    display.println("NPN Transistor:");
    display.println("C = Right");
    display.println("E = Left");
    detected = true;
  }

  if (!detected) {
    display.println("No conduction");
    display.println("Not NPN or damaged");
  }

  display.display();

}

void check_pnp(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("PNP Transistor Test");

  bool detected = false;

  // ----- Test 1: Collector = Left (L), Emitter = Right (R) -----
  pinMode(L, OUTPUT); digitalWrite(L, HIGH); // Collector = HIGH
  pinMode(R, INPUT);
  pinMode(M, OUTPUT); digitalWrite(M, HIGH); // Base = HIGH
  delay(10);
  bool voltage1 = digitalRead(R);  // Should be LOW if current flows

  pinMode(L, OUTPUT); digitalWrite(L, HIGH); // Collector = HIGH
  pinMode(R, INPUT);
  pinMode(M, OUTPUT); digitalWrite(M, LOW); // Base = HIGH
  delay(10);
  bool voltage01 = digitalRead(R);  // Should be LOW if current flows

  if (voltage1 == false && voltage01 == true) {
    display.println("PNP Transistor:");
    display.println("C = Left");
    display.println("E = Right");
    detected = true;
  }

  // ----- Test 2: Collector = Right (R), Emitter = Left (L) -----
  pinMode(L, INPUT); 
  pinMode(R, OUTPUT); digitalWrite(R, HIGH); // Collector = HIGH
  pinMode(M, OUTPUT); digitalWrite(M, HIGH); // Base = HIGH
  delay(10);
  bool voltage2 = digitalRead(L);  // Should be LOW if current flows

  pinMode(L, INPUT); 
  pinMode(R, OUTPUT); digitalWrite(R, HIGH); // Collector = HIGH
  pinMode(M, OUTPUT); digitalWrite(M, LOW); // Base = HIGH
  delay(10);
  bool voltage02 = digitalRead(L);  // Should be LOW if current flows

  if (!detected && voltage2 == false && voltage02 == true) {
    display.println("PNP Transistor:");
    display.println("C = Right");
    display.println("E = Left");
    detected = true;
  }

  if (!detected) {
    display.println("No conduction");
    display.println("Not PNP or damaged");
  }

  display.display();

}

void TEST(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Test");

  bool detected = false;
  // ----- Test 1: Collector = Left (L), Emitter = Right (R) -----
  pinMode(L, OUTPUT); digitalWrite(L, HIGH); // Collector = HIGH
  pinMode(R, INPUT);
  pinMode(M, OUTPUT); digitalWrite(M, HIGH); // Base = HIGH
  delay(10);
  bool voltage10 = digitalRead(R);  // Should be LOW if current flows

  pinMode(L, OUTPUT); digitalWrite(L, HIGH); // Collector = HIGH
  pinMode(R, INPUT);
  pinMode(M, OUTPUT); digitalWrite(M, LOW); // Base = HIGH
  delay(10);
  bool voltage010 = digitalRead(R);  // Should be LOW if current flows

  if (voltage10 == false && voltage010 == true) {
    display.println("PNP Transistor:");
    display.println("C = Left");
    display.println("E = Right");
    detected = true;
  }

  // ----- Test 2: Collector = Right (R), Emitter = Left (L) -----
  pinMode(L, INPUT); 
  pinMode(R, OUTPUT); digitalWrite(R, HIGH); // Collector = HIGH
  pinMode(M, OUTPUT); digitalWrite(M, HIGH); // Base = HIGH
  delay(10);
  bool voltage20 = digitalRead(L);  // Should be LOW if current flows

  pinMode(L, INPUT); 
  pinMode(R, OUTPUT); digitalWrite(R, HIGH); // Collector = HIGH
  pinMode(M, OUTPUT); digitalWrite(M, LOW); // Base = HIGH
  delay(10);
  bool voltage020 = digitalRead(L);  // Should be LOW if current flows

  if (!detected && voltage20 == false && voltage020 == true) {
    display.println("PNP Transistor:");
    display.println("C = Right");
    display.println("E = Left");
    detected = true;
  }

  // ----- Test 1: Collector = Left (L), Emitter = Right (R) -----
  pinMode(L, OUTPUT); digitalWrite(L, HIGH); // Collector = HIGH
  pinMode(R, INPUT);
  pinMode(M, OUTPUT); digitalWrite(M, HIGH); // Base = HIGH
  delay(10);
  bool voltage1 = digitalRead(R);  // Should be LOW if current flows

  pinMode(L, OUTPUT); digitalWrite(L, HIGH); // Collector = HIGH
  pinMode(R, INPUT);
  pinMode(M, OUTPUT); digitalWrite(M, LOW); // Base = HIGH
  delay(10);
  bool voltage01 = digitalRead(R);  // Should be LOW if current flows

  if (!detected && voltage1 == true && voltage01 == false) {
    display.println("NPN Transistor:");
    display.println("C = Left");
    display.println("E = Right");
    detected = true;
  }

  // ----- Test 2: Collector = Right (R), Emitter = Left (L) -----
  pinMode(L, INPUT); 
  pinMode(R, OUTPUT); digitalWrite(R, HIGH); // Collector = HIGH
  pinMode(M, OUTPUT); digitalWrite(M, HIGH); // Base = HIGH
  delay(10);
  bool voltage2 = digitalRead(L);  // Should be LOW if current flows

  pinMode(L, INPUT); 
  pinMode(R, OUTPUT); digitalWrite(R, HIGH); // Collector = HIGH
  pinMode(M, OUTPUT); digitalWrite(M, LOW); // Base = HIGH
  delay(10);
  bool voltage02 = digitalRead(L);  // Should be LOW if current flows

  if (!detected && voltage2 == true && voltage02 == false) {
    display.println("NPN Transistor:");
    display.println("C = Right");
    display.println("E = Left");
    detected = true;
  }

  if (!detected) {
    display.println("No conduction");
    display.println("Not NPN/PNP or damaged");
  }

  display.display();
}

void measureVoltage() {
    Serial.println("Measuring Voltage...");
    
    while (true) {  
        displayModes1();  
        
        // UP Button (Scroll Up)
        delay(200);
        if (digitalRead(UP_BUTTON) == HIGH) {
            if (mode_index1 > 0) {
                mode_index1--;
                delay(200);  // Debounce delay
            }
        }

        // DOWN Button (Scroll Down)
        if (digitalRead(DOWN_BUTTON) == HIGH) {
            if (mode_index1 < total_modes1 - 1) {
                mode_index1++;
                delay(200);  // Debounce delay
            }
        }

        // NEXT Button (Select Mode)
        if (digitalRead(NEXT_BUTTON) == HIGH) {
            executeMode1(mode_index1);
            break; 
        }
        
        delay(50);  // Small delay to prevent overwhelming the CPU
    }
}

void measureCurrent() {
 ACS.autoMidPoint();  // Automatically calibrate the zero-current voltage

  while (true) {
    int current_mA = ACS.mA_DC();  // Measure DC current in milliamps

    Serial.print("Current: ");
    Serial.print(current_mA);
    Serial.println(" mA");

    // Optional: Display on OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Current: ");
    display.print(current_mA);
    display.println(" mA");
    display.display();

    delay(200);
  }

}

void measureVoltage3() {
    while(true){
      int adcValue = analogRead(VOLTAGE_PIN);
  float vNode = adcValue * (ADC_REF / ADC_RES);  // Voltage at GPIO15
  float vin = vNode * (1.0 + (R04 / R02));          // Calculate original input voltage

  Serial.print("ADC Value: "); Serial.println(adcValue);
  Serial.print("Vnode: "); Serial.print(vNode, 3); Serial.println(" V");
  Serial.print("Vin (calculated): "); Serial.print(vin, 3); Serial.println(" V");

  // Optional OLED display
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Vin: ");
  display.print(vin, 2);
  display.println(" V");
  display.display();
  delay(200);
    }
}

void measureVoltage9() {
    while(true){
      int adcValue = analogRead(VOLTAGE_PIN);
  float vNode = adcValue * (ADC_REF / ADC_RES);  // Voltage at GPIO15
  float vin = vNode * (1.0 + (R01 / R02));          // Calculate original input voltage

  Serial.print("ADC Value: "); Serial.println(adcValue);
  Serial.print("Vnode: "); Serial.print(vNode, 3); Serial.println(" V");
  Serial.print("Vin (calculated): "); Serial.print(vin, 3); Serial.println(" V");

  // Optional OLED display
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Vin: ");
  display.print(vin, 2);
  display.println(" V");
  display.display();
  delay(200);
    }
}

void measureVoltage15() {
    while(true){
      int adcValue = analogRead(VOLTAGE_PIN);
  float vNode = adcValue * (ADC_REF / ADC_RES);  // Voltage at GPIO15
  float vin = vNode * (1.0 + (R03 / R02));          // Calculate original input voltage

  Serial.print("ADC Value: "); Serial.println(adcValue);
  Serial.print("Vnode: "); Serial.print(vNode, 3); Serial.println(" V");
  Serial.print("Vin (calculated): "); Serial.print(vin, 3); Serial.println(" V");

  // Optional OLED display
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Vin: ");
  display.print(vin, 2);
  display.println(" V");
  display.display();
  delay(200);
    }
}


void measureResistance() {
  while (true) {
    float Vout, R_unknown;
    String label = "Invalid";

    // Lambda functions
    auto activate = [](int pin) {
      pinMode(BASE1, INPUT);
      pinMode(BASE2, INPUT);
      pinMode(BASE3, INPUT);
      pinMode(pin, OUTPUT);
      digitalWrite(pin, HIGH);
    };

    auto readVoltage = []() {
      int adcValue = analogRead(ADC_PIN);
      return adcValue * (ADC_REF / ADC_RES);
    };

    auto calcResistance = [](float Vout, float R_known) -> float {
      if (Vout <= 0.01 || Vout >= V_IN) return -1.0;
      return (R_known * Vout) / (V_IN - Vout);
    };

    // Try 1k
    activate(BASE1);
    delay(100);
    Vout = readVoltage();
    R_unknown = calcResistance(Vout, R1);
    if (R_unknown > 0 && R_unknown < 8000) label = "1k Ref";
    else {
      activate(BASE2);
      delay(100);
      Vout = readVoltage();
      R_unknown = calcResistance(Vout, R2);
      if (R_unknown >= 8000 && R_unknown < 80000) label = "10k Ref";
      else {
        activate(BASE3);
        delay(100);
        Vout = readVoltage();
        R_unknown = calcResistance(Vout, R3);
        if (R_unknown >= 80000 && R_unknown < 150000) label = "100k Ref";
        else R_unknown = -1;
      }
    }

    pinMode(BASE1, INPUT);
    pinMode(BASE2, INPUT);
    pinMode(BASE3, INPUT);

    Serial.println("------");
    display.clearDisplay();

    if (R_unknown > 0) {
      Serial.print("Ref Used: "); Serial.println(label);
      Serial.print("Voltage: "); Serial.println(Vout);
      Serial.print("Resistance: "); Serial.print(R_unknown); Serial.println(" ohms");

      display.setCursor(0, 0);
      display.println("Resistance Measured:");
      display.setCursor(0, 10);
      display.print(R_unknown, 0);
      display.println(" ohms");
      display.setCursor(0, 22);
      display.print("Using "); display.println(label);
    } else {
      Serial.println("No valid range.");
      display.setCursor(0, 0);
      display.println("Invalid Range!");
      display.setCursor(0, 10);
      display.println("Check connection");
    }

    display.display();
    delay(1000);
  }
}


void measureCapacitance() {
  while(true){


  const int chargePin = 5;
  const int dischargePin = 2;
  const int analogPin = 34;
  const float Rc = 1500.0;  // 1.5k Ohm
  const int ADC_MAX = 4095;
  const float Vcc = 3.3;
  const int targetADC = (int)(0.63 * ADC_MAX);  // ~2580 for 3.3V

  Serial.println("Measuring Capacitance...");

  // Step 1: Discharge the capacitor
  pinMode(chargePin, OUTPUT);
  pinMode(dischargePin, OUTPUT);
  digitalWrite(chargePin, LOW);
  digitalWrite(dischargePin, LOW);
  delay(500);

  // Step 2: Begin charging
  pinMode(dischargePin, INPUT);   // Disconnect discharge
  digitalWrite(chargePin, HIGH);  // Start charging

  // Step 3: Time until 63% Vcc is reached
  unsigned long startTime = micros();
  while (analogRead(analogPin) < targetADC) {
    if ((micros() - startTime) > 3000000) break;  // Timeout after 3s
  }
  unsigned long elapsedTime = micros() - startTime;

  // Step 4: Calculate capacitance in µF
  float capacitance = (float)elapsedTime / Rc;
  capacitance *= 0.7692; // µF, because time is in µs and R in ohms

  // Debug output
  Serial.print("Elapsed Time: ");
  Serial.print(elapsedTime);
  Serial.println(" us");

  Serial.print("Capacitance: ");
  Serial.print(capacitance, 3);
  Serial.println(" uF");

  // OLED Display Output
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Capacitance:");
  display.setCursor(0, 10);
  display.print(capacitance, 2);
  display.println(" uF");
  display.display();

  delay(2000);
  }
}

void continuity() {
    analogReadResolution(12);   // 12-bit ADC (0–4095)
  const float ADC_REF = 3.3;  // Reference voltage
  const int ADC_RES = 4095;

  while (true) {
    int adcValue = analogRead(CONTINUITY_PIN);
    float voltage = adcValue * (ADC_REF / ADC_RES);

    Serial.print("Voltage: ");
    Serial.print(voltage, 3);
    Serial.print(" V - ");

    if (voltage > CONTINUITY_THRESHOLD) {
      Serial.println("Continuity Detected!");
      
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Continuity OK");
      display.display();
    } else {
      Serial.println("No Continuity");
      
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("No Continuity");
      display.display();
    }

    delay(300);  // Delay between readings
  }
}


void testIC() {
    while (true) {  
        displayModes3();  
        
        // UP Button (Scroll Up)
        delay(200);
        if (digitalRead(UP_BUTTON) == HIGH) {
            if (mode_index3 > 0) {
                mode_index3--;
                delay(200);  // Debounce delay
            }
        }

        // DOWN Button (Scroll Down)
        if (digitalRead(DOWN_BUTTON) == HIGH) {
            if (mode_index3 < total_modes3 - 1) {
                mode_index3++;
                delay(200);  // Debounce delay
            }
        }

        // NEXT Button (Select Mode)
        if (digitalRead(NEXT_BUTTON) == HIGH) {
            executeMode3(mode_index3);
            break; 
        }
        
        delay(50);  // Small delay to prevent overwhelming the CPU
    }
}

void testTransistor() {
    while (true) {  
        displayModes2();  
        
        // UP Button (Scroll Up)
        delay(200);
        if (digitalRead(UP_BUTTON) == HIGH) {
            if (mode_index2 > 0) {
                mode_index2--;
                delay(200);  // Debounce delay
            }
        }

        // DOWN Button (Scroll Down)
        if (digitalRead(DOWN_BUTTON) == HIGH) {
            if (mode_index2 < total_modes2 - 1) {
                mode_index2++;
                delay(200);  // Debounce delay
            }
        }

        // NEXT Button (Select Mode)
        if (digitalRead(NEXT_BUTTON) == HIGH) {
            executeMode2(mode_index2);
            break; 
        }
        
        delay(50);  // Small delay to prevent overwhelming the CPU
    }
}


void setup() {
    Serial.begin(115200);
    
    // Initialize OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 allocation failed");
        for (;;);
    }

    // Configure button inputs (Active High)
    pinMode(UP_BUTTON, INPUT);
    pinMode(DOWN_BUTTON, INPUT);
    pinMode(NEXT_BUTTON, INPUT);

    displayModes0(); // Show initial mode list
}

void loop() {
    // UP Button (Scroll Up)
    buttons();
}