#include <util/atomic.h> // For the ATOMIC_BLOCK macro
#define ENCA 2 // 
#define ENCB 3 //
#define PWM 7

const int IN1 = 8;   
const int IN2 = 9;  
bool dir;            
int ri;
volatile int posi = 0; 
long pwr;



void setup() {  

  Serial.begin(9600);   
  pinMode (IN1, OUTPUT);   
  pinMode (IN2, OUTPUT);
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);   
  ri=0;
  dir = true;  
}


void loop() {
  int pos = 0; 
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    pos = posi;
  }
  
  pwr = map(abs(pos), 0, 200, 180, 180); //wenn der Hebel sich der Position 0 nähert soll er auf die Leistung 35 schalten, wenn er sich dem Punkt 200 nähert soll er bis zu diesem Punkt mit der Leistung linear ansteigen bis zum Leistungswert 255
  if (posi % 10 == 0) {                      //wenn Position kleiner als -1 also -2,-3.... 
    analogWrite(IN1, LOW);              //...dann gilt für IN1 die Befehle der Map Funktion 
    digitalWrite(IN2, pwr);             //...in andere Richtung ist aus  
  } 
  else {
    digitalWrite(IN1, LOW);            //im Bereich -1 bis 1 soll der Motor stehen (im 0 Punkt)
    digitalWrite(IN2, LOW);
  }

  
  Serial.println(pos);
}


void readEncoder(){                    
  int b = digitalRead(ENCB);
  if(b > 0){
    posi++;
  }
  else{
    posi--;
  }
}
