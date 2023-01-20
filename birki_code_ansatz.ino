#include <util/atomic.h> // For the ATOMIC_BLOCK macro
#define ENCA 2 // 
#define ENCB 3 //
#define aktuellesjahr 2022
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
  
  pwr = map(abs(pos), 0, 200, 50, 150);
  if (posi < -1) {
    analogWrite(IN2, pwr);
    digitalWrite(IN1, LOW);
  } 
  else if (posi > 1) {
    analogWrite(IN1, pwr);
    digitalWrite(IN2, LOW);
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
