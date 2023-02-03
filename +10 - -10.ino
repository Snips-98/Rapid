#include <util/atomic.h> // For the ATOMIC_BLOCK macro
#define ENCA 2 // 
#define ENCB 3 //
#define PWM 7

const int IN1 = 8;   
const int IN2 = 9;            
volatile int posi = 0; 



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
  
  if (posi % 10 == 0) {                  
    analogWrite(IN1, LOW);              
    digitalWrite(IN2, 255);    
  } 
  else {
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
