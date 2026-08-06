#define BUTTON_PIN 2

const byte V_STOP = 13;
const byte V_CAUTION = 12;
const byte V_PROCEED = 11;

const byte P_STOP = 10;
const byte P_PROCEED = 9;

void setup() {
  pinMode(V_STOP, OUTPUT); //red for vehicles
  pinMode(V_CAUTION, OUTPUT); //yellow for vehicles
  pinMode(V_PROCEED, OUTPUT); //green for vehicles

  pinMode(P_STOP, OUTPUT); //red for pedestrian
  pinMode(P_PROCEED, OUTPUT); //green for pedestrian

  pinMode(BUTTON_PIN, INPUT_PULLUP);//button (when button not pressed, it is HIGH)

  Serial.begin(9600);
  Serial.println("start program");

digitalWrite(V_STOP, LOW);
  digitalWrite(V_CAUTION, LOW);
  digitalWrite(V_PROCEED, HIGH);

  digitalWrite(P_STOP, HIGH);
  digitalWrite(P_PROCEED, LOW);
}

void loop() {

  byte buttonState = digitalRead(BUTTON_PIN); 

  if (buttonState == LOW) {
    Serial.println("lesgo");
    delay(2000);
    digitalWrite(V_PROCEED, LOW);
    digitalWrite(V_CAUTION, HIGH);
    delay(3000);
    digitalWrite(V_CAUTION, LOW);
    digitalWrite(V_STOP, HIGH);
    delay(2000);

    digitalWrite(P_STOP, LOW);
    digitalWrite(P_PROCEED, HIGH);
    delay(5000);
    
    for(int i = 0; i < 5; i++){
      digitalWrite(P_PROCEED, LOW);
      delay(500);
      digitalWrite(P_PROCEED, HIGH);
      delay(500);
    }

  digitalWrite(P_STOP, HIGH);
  digitalWrite(P_PROCEED, LOW);

  delay(2000);
  digitalWrite(V_STOP, HIGH);
  digitalWrite(V_PROCEED, LOW);

  } else {
    Serial.println("bro press the button");
  }
  
  delay(100); // Small delay to stabilize  
}