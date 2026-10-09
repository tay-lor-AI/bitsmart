// Pinouts for 2x 5161AS or 1/2 5461AS (Common Cathode)
//                            A   B   C   D   E   F   G  DP
const byte pinSegments[] = { 11,  7,  4,  2, 13, 10,  5,  3};
const byte digits[]      = { 12,  9,  8,  6};

const byte SENSOR  = A0;
const byte MIN_SET = A1;
const byte MAX_SET = A2;

// Patterns 0-9, 10 = OFF, 11 = MINUS
const byte patterns[] = { 
  0b01111110, // 0 
  0b00110000, // 1 
  0b01101101, // 2
  0b01111001, // 3
  0b00110011, // 4
  0b01011011, // 5
  0b01011111, // 6
  0b01110000, // 7
  0b01111111, // 8
  0b01111011, // 9
  0b00000000, // 10 (Off)
  0b00000001  // 11 (Minus sign)
};

unsigned long lastPrintTime = 0;

void setup() {
  for (byte i = 0; i < 8; i++) {
    pinMode(pinSegments[i], OUTPUT);
  }
  for (byte i = 0; i < 4; i++) {
    pinMode(digits[i], OUTPUT);
    digitalWrite(digits[i], HIGH); // HIGH = OFF for Common Cathode
  }
  Serial.begin(9600);
}

void loop() {
  int sensor_val = analogRead(SENSOR);
  int min_val = analogRead(MIN_SET);
  int max_val = analogRead(MAX_SET);

  if (min_val > max_val) {
    int temp = min_val;
    min_val = max_val;
    max_val = temp;
  }
  
  if (min_val == max_val) {
    max_val = min_val + 1;
  }

  sensor_val = constrain(sensor_val, min_val, max_val);
  int level  = map(sensor_val, min_val, max_val, -500, 500);

  refreshDisplay(level);

  //Print to Serial non-blockingly every 500ms
  if (millis() - lastPrintTime >= 500) {
    lastPrintTime = millis();
    Serial.print("Sensor: ");
    Serial.print(sensor_val);
    Serial.print(" | Level: ");
    Serial.println(level);
  }
  // int raw_sensor = analogRead(SENSOR);
  // int raw_min    = analogRead(MIN_SET);
  // int raw_max    = analogRead(MAX_SET);

  // Serial.print("Raw LDR (A0): ");
  // Serial.print(raw_sensor);
  // Serial.print(" | Min Pot (A1): ");
  // Serial.print(raw_min);
  // Serial.print(" | Max Pot (A2): ");
  // Serial.println(raw_max);
}

// Multiplexes through all 4 digits quickly
void refreshDisplay(int num) {
  int absNum   = abs(num);
  int hundreds = (absNum / 100) % 10;
  int tens = (absNum / 10) % 10;
  int ones = absNum % 10;

  for (byte digit = 0; digit < 4; digit++) {
    byte patternIdx = 10; // Default to OFF

    if (digit == 0) {
      if (num < 0) patternIdx = 11; // Minus sign
    } else if (digit == 1) {
      if (absNum >= 100) patternIdx = hundreds;
    } else if (digit == 2) {
      if (absNum >= 10) patternIdx = tens;
    } else if (digit == 3) {
      patternIdx = ones;
    }

    for (byte i = 0; i < 4; i++) {
      digitalWrite(digits[i], HIGH); 
    }

    // Write segments (HIGH = ON)
    byte mask = 0b01000000;
    for (byte i = 0; i < 7; i++) {
      digitalWrite(pinSegments[i], (patterns[patternIdx] & mask) ? HIGH : LOW);
      mask >>= 1;
    }
    digitalWrite(pinSegments[7], LOW); // DP off

    // Turn ON current digit (LOW = ON for Common Cathode)
    digitalWrite(digits[digit], LOW);
    delayMicroseconds(1000); 
  }
}