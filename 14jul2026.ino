void setup() {
  // put your setup code here, to run once:
  //pin 13 is the LED
  pinMode(13, OUTPUT);
  pinMode(12, INPUT);
  Serial.begin(9600);
  Serial.println("Hello World!");

}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(12) == HIGH){
     Serial.println("HIGH");
    digitalWrite(13, HIGH);
    delay(250);
    digitalWrite(13, LOW);
    delay(750);
  }
  else{
    Serial.println("LOW");
    digitalWrite(13, HIGH);
    delay(25);
    digitalWrite(13, LOW);
    delay(75);
  }
  
}
