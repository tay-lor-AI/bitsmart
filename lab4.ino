    // Pinouts for 2x 5161AS or 1/2 5461AS
// but in array form.

//                            A   B   C   D   E   F   G  DP
const byte pinSegments[] = { 11,  7, 4,  2, 13,  10, 5, 3};

// Digit select pins:
// digit 0 = sign/thousands position
// digit 1 = hundreds
// digit 2 = tens
// digit 3 = ones
const byte digits[] ={ 12, 9, 8, 6};	// 12 is the negative sign
const byte SENSOR = A0;
const byte MIN_SET = A1;
const byte MAX_SET = A2;
// This array contains bitmap patterns that will be displayed
// to segments A through G. The DP is controlled independently.
// 1 is lit, 0 is unlit

//                        0b0ABCDEFG  <-- Binary number
const byte patterns[] = { 0b01111110,	// 0 - 
                          0b00110000,	// 1 - 
                          0b01101101,	// 2
                          0b01111001,	// 3
                          0b00110011,	// 4
                          0b01011011,	// 5
                          0b01011111,	// 6
                          0b01110000,	// 7
                          0b01111111,	// 8
                          0b01111011,	// 9
                          0b00000000, // 10 everything is off
                          0b11111111}; //11 everything is on
const byte maxPatterns = 11; // maximum index number for patterns[]
const byte minusSign[] = {0b00000001};
const byte noSign[] = {0b00000000};
const byte BLANK = 10;

void display(byte patternIndex, byte digit, bool dp)
{
  // This performs bitwise operations to display the
  // bit pattern to the assigned digit.
  
  // First, turn off all the digits
  // digitalWrite(digits[0], HIGH);
  // digitalWrite(digits[1], HIGH);
  
  // Set the bitmap pattern to the pins]
  // mask is used to check which bit to display

    // turn everything off plsplspls
    for (byte i = 0; i < 4; i++) {
    digitalWrite(digits[i], HIGH);
    }
  byte mask = 0b01000000;
  
  //Go through 7 segments
  for (byte i = 0; i < 7; i++)
  {
    // Bitwise-AND the mask with the pattern.
    // AND needs both parts to be 1 to return a 1.
    // Any non-zero result is HIGH, otherwise it's LOW
    digitalWrite(pinSegments[i], (patterns[patternIndex] & mask) ? HIGH : LOW);
    // Move the mask one bit to the right to get the next segment
    mask >>= 1;
  }
  
  // Decimal point
  digitalWrite(pinSegments[7], dp ? HIGH : LOW);

  // Turn ON selected digit.
  digitalWrite(digits[digit], LOW);
}

void displayMinus(byte digit)
{
  // Turn OFF all digits first.
  for (byte i = 0; i < 4; i++) {
    digitalWrite(digits[i], HIGH);
  }

  // Turn OFF all segments
  for (byte i = 0; i < 8; i++) {
    digitalWrite(pinSegments[i], LOW);
  }

  // Segment G = minus sign
  digitalWrite(pinSegments[6], HIGH);

  // Turn selected digit ON
  digitalWrite(digits[digit], LOW);
}

void setup()
{
  // Configure the segments and digit anodes to outputs
  for (byte i = 0; i < 8; i++)
  	pinMode(pinSegments[i], OUTPUT);
  
  pinMode(digits[0], OUTPUT);
  pinMode(digits[1], OUTPUT);
  pinMode(digits[2], OUTPUT);
  pinMode(digits[3], OUTPUT);
  
  // Turn both displays off
  digitalWrite(digits[0], HIGH);
  digitalWrite(digits[1], HIGH);
  digitalWrite(digits[2], HIGH);
  digitalWrite(digits[3], HIGH);

    Serial.begin(9600);
    Serial.println("STARTING...");
}

void loop()
  {
    //potentiometers <3
    int min_val = analogRead(MIN_SET);
    int max_val = analogRead(MAX_SET);

 if (min_val > max_val) {
    int temp = min_val;
    min_val = max_val;
    max_val = temp;
  }

    //read potentiometer tyshii
  int sensor_val = analogRead(SENSOR);

  // Prevent division/mapping problems.
  if (min_val == max_val) {
    min_val = 0;
    max_val = 1023;
  }
    //constrains value to min and max
    int sensor_val = constrain(sensor_val, min_val, max_val);
    //maps the value from levels 0-5
    int level = map(sensor_val, min_val, max_val, -500, 500);
      
    //long rand = random(-500, 501);

    displayNum(level);
    Serial.println("displayed " + level);

  }

void displayNum(int num){
    int absNum = abs(num);

    // them digits
    int hund = (absNum / 100) % 10;
    int tens = (absNum / 10) % 10;
    int ones = absNum % 10;

    // negative sign
    if (num < 0) {
        displayMinus(0);    
    } else {
      display(BLANK, 0, false);
    }
    
    delayMicroseconds(1000);

    // hundreds
    if(absNum >= 100){
      display(hund, 1, false);
    } else {
      display(BLANK, 1, false );
    }
    
    delayMicroseconds(1000);

    // tens
     if (absNum >= 10) {
    display(tens, 2, false);
  }
  else {
    display(BLANK, 2, false);
  }

  delayMicroseconds(1000);

    // ones
  display(ones, 3, false);

  delayMicroseconds(1000);
}
