const byte V_STOP = 13;
const byte V_CAUTION = 12;
const byte V_PROCEED = 11;

const byte P_STOP = 10;
const byte P_PROCEED = 9;

const byte V_BUT = 3;
const byte P_BUT = 2;

volatile int P_COUNT = 0;
volatile int V_COUNT = 0;

void pedestrianInterrupt() {
P_COUNT++;
Serial.print("plus one pedestrian");
}

void vehicleInterrupt() {
V_COUNT++;
Serial.print("plus one vehicle");
}

void setup() {
  pinMode(V_STOP, OUTPUT); //red for vehicles
  pinMode(V_CAUTION, OUTPUT); //yellow for vehicles
  pinMode(V_PROCEED, OUTPUT); //green for vehicles

  pinMode(P_STOP, OUTPUT); //red for pedestrian
  pinMode(P_PROCEED, OUTPUT); //green for pedestrian

  pinMode(V_BUT, INPUT_PULLUP);//button (when button not pressed, it is HIGH)
  pinMode(P_BUT, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(2), pedestrianInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), vehicleInterrupt, FALLING);
  
  Serial.begin(9600);
  Serial.println("start program");

//  A. Starting condition for 10 seconds

	digitalWrite(V_STOP, LOW);	
  	digitalWrite(V_CAUTION, LOW);
  	digitalWrite(V_PROCEED, HIGH);

    digitalWrite(P_STOP, HIGH);
    digitalWrite(P_PROCEED, LOW);

}

void loop() {

   Serial.println("Vehicles GOGOGO");
   delay(10000);
  
// #9
  int V_EXTIME = (V_COUNT / 5) * 2000;
     
 if (V_EXTIME > 10000){
   V_EXTIME = 10000;
 }
  
 if (V_EXTIME > 0){
   Serial.print("Vehicle extra time: ");
   Serial.print(V_EXTIME / 1000);
   Serial.println(" seconds");
   delay(V_EXTIME);
 }    
  
  V_COUNT = 0;
  
  
// B. Signal vehicles to caution for 3 seconds.
 Serial.println("Vehicles caution");

  digitalWrite(V_PROCEED, LOW);
  digitalWrite(V_CAUTION, HIGH);

  delay(3000);
  
// C. Signal vehicles to stop.
  Serial.println("Vehicles stop PRETTY PLEASE MAMACITA");

  digitalWrite(V_CAUTION, LOW);
  digitalWrite(V_STOP, HIGH);

// D. Signal pedestrians to proceed after 2 seconds.
  delay(2000);
  
  digitalWrite(P_STOP, LOW);
  digitalWrite(P_PROCEED, HIGH);
  
  Serial.println("PE proceed");
  int P_EXTIME = P_COUNT * 2000;

// #6
  if (P_EXTIME > 10000){
   	P_EXTIME = 10000;
   }
// E. Keep pedestrians going for 5 seconds.
  delay(5000);
  
  if (P_EXTIME > 0) {
    Serial.print("Pedestrian extra time: ");
   Serial.print(P_EXTIME / 1000);
   Serial.println(" seconds");
    delay(P_EXTIME);
  }
  
  P_COUNT = 0;
  
// F. Flash the pedestrian proceed signal for 5 seconds at a rate of one flash (on and off cycle) per second.

    for(int i = 0; i < 5; i++){
      digitalWrite(P_PROCEED, LOW);
      delay(500);
        Serial.println("PED flash wait");
      digitalWrite(P_PROCEED, HIGH);
      delay(500);
    }
 // G. Signal pedestrians to stop.
  digitalWrite(P_STOP, HIGH);
  digitalWrite(P_PROCEED, LOW);

 // H. Signal vehicles to proceed after 2 seconds.
  delay(2000);
  Serial.println("VEH proceed");
  digitalWrite(V_STOP, LOW);
  digitalWrite(V_PROCEED, HIGH);
}