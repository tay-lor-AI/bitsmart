    // Pinouts for 2x 5161AS or 1/2 5461AS
// but in array form.

//                            A   B   C   D   E   F   G  DP
const byte pinSegments[] = { 11,  7, 4,  2, 13,  10, 5, 3};
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

void display(byte patternIndex, byte digit, bool dp)
{
  // This performs bitwise operations to display the
  // bit pattern to the assigned digit.
  
  // First, turn off all the digits
  // digitalWrite(digits[0], HIGH);
  // digitalWrite(digits[1], HIGH);
  
  // Set the bitmap pattern to the pins]
  // mask is used to check which bit to display
  byte mask = 0b01000000;
  
  //Go through 7 segments
  for (byte i = 0; i < 7; i++)
  {
    // Bitwise-AND the mask with the pattern.
    // AND needs both parts to be 1 to return a 1.
    // Any non-zero result is HIGH, otherwise it's LOW
    digitalWrite(pinSegments[i], patterns[patternIndex] & mask);
    // Move the mask one bit to the right to get the next segment
    mask >>= 1;
  }
  
  // Set the decimal point
  digitalWrite(pinSegments[7], dp);

  // Finally, turn on the specified digit
  digitalWrite(digits[digit], LOW);
}

unsigned long lastTime = 0;
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
  digitalWrite(digits[0], LOW);
  digitalWrite(digits[1], LOW);
  digitalWrite(digits[2], LOW);
  digitalWrite(digits[3], LOW);
}

void loop()
  {
    //potentiometers <3
    int min_val = analogRead(MIN_SET);
    int max_val = analogRead(MAX_SET);

    if (min_val > max_val) {
    int meow = min_val;
    min_val = max_val;
    max_val = meow;
    }

    //constrains value to min and max
    int sensor_val = constrain(sensor_val, min_val, max_val);
    //maps the value from levels 0-5
    int level = map(sensor_val, min_val, max_val, -500, 500);
      
    //long rand = random(-500, 501);

    displayNum(level);
    Serial.println("displayed" + level);

  }

void displayNum(int num){
    int absNum = abs(num);

    // them digits
    int hund = (absNum / 100) % 10;
    int tens = (absNum / 10) % 10;
    int ones = absNum % 10;

    if (num < 0) {
      display(minusSign, 12, false);
    } else {
      display(noSign, 12, false);
    }

    if(absNum < 100 && num >= 0){
      display(0, 9, false);
    } else {
      display(patterns[hund], digits[1], false );
    }

    if(absNum < 10 && num >= 0) {
      display(0, 8, noSign);
    } else {
      display(patterns[tens], digits[2], false);
    }

    display(patterns[ones], digits[3], false);
    Serial.println(num);

  }