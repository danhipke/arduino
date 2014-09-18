const int TRIG = 2;

void setup() {
  Serial.begin(9600);
  
}

void loop() {
  long duration, inches, cm;
  
  pinMode(TRIG, OUTPUT);
  pinMode(A0, INPUT);
  
  digitalWrite(TRIG, LOW);
  delay(2);
  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(A0, HIGH);
  
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  delay(100);
  
     
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;  
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;  
}
