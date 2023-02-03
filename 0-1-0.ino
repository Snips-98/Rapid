#include <util/atomic.h> // For the ATOMIC_BLOCK macro
#define ENCA 2 
#define ENCB 3 
#define PWM 7

const int IN1 = 8;   
const int IN2 = 9;  
volatile int posi = 0; 
long pwr;


void setup() {  

  Serial.begin(9600);   
  pinMode (IN1, OUTPUT);   
  pinMode (IN2, OUTPUT);
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);    
}


void loop() {
  int pos = 0; 
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    pos = posi;
  }
  
  pwr = map(abs(pos), 0, 80, 55, 180); //wenn der Hebel sich der Position 0 nähert soll er auf die Leistung 55 schalten, wenn er sich der Position 80 nähert soll er bis zu diesem Punkt mit der Leistung linear ansteigen bis zum Leistungswert 180

  if (posi < -1) {                      //wenn Position kleiner als -1 also -2,-3.... 
    analogWrite(IN1, pwr);              //...dann gilt für IN1 die Befehle der Map Funktion 
    digitalWrite(IN2, LOW);             //...in andere Richtung ist aus  
  } 
  else if (posi > 1) {                  //wenn Position größer als 1.... 
    analogWrite(IN2, pwr);              //...dann gilt für IN1 die Befehle der Map Funktion
    digitalWrite(IN1, LOW);             //...in andere Richtung ist aus
  } 
  else {                                //wenn Poition 0 dann ist der motor aus
    digitalWrite(IN1, LOW);
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
