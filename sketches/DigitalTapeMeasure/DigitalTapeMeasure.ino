#include "SevSeg.h"

//Create an instance of the object.
SevSeg sevseg;

const int TRIG = 2;

void setup() {
  //I am using a common anode display, with the digit pins connected
  //from 2-5 and the segment pins connected from 6-13
  sevseg.Begin(0,1,3,4,5,6,7,8,9,10,11,12,13);
  //Set the desired brightness (0 to 100);
  sevseg.Brightness(90);
  Serial.begin(9600);
  
}

void loop() {
  long duration, inches, cm;
  
  sevseg.PrintOutput();
  
  pinMode(TRIG, OUTPUT);
  pinMode(A0, INPUT);
  
  digitalWrite(TRIG, LOW);
  delay(1);
  digitalWrite(TRIG, HIGH);
  delay(3);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(A0, HIGH);
  
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  sevseg.NewNum(cm, (byte) 3);
  //delay(1);
  
     
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;  
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;  
}
