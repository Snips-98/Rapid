#include <util/atomic.h> // For the ATOMIC_BLOCK macro
#define ENCA 2 // 
#define ENCB 3 //
#define PWM 7

const int IN1 = 8;   
const int IN2 = 9;  
volatile int posi = 0; 
long pwr;
long pwr2;
long pwr3;
long pwr4;


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
  
  pwr3 = map(pos, -60, 0, 185, 55); //sorgt dafür das der Joystick nicht über 0 hinaus fährt
  pwr4 = map(pos, 160, 100, 185, 55); //sorgt dafür das der Joystick nicht über 100 hinausföhrt
  pwr2 = map(abs(pos), 50, 100, 180, 15);
  pwr = map(abs(pos), 0, 50, 35, 180);
       
  if (posi < -1) {                     //wenn Position kleiner als -1 fährt der Motor richtung 0
    digitalWrite(IN2, LOW);             
    analogWrite(IN1, pwr3);             
  } 
  else if (posi <= 50) {               //wenn Position kleiner gleich 50 drückt der Motor richtung 0
    digitalWrite(IN1, LOW);           
    analogWrite(IN2, pwr);             
  } 
  else if(posi > 50) {
    digitalWrite(IN2, LOW);            //wenn Position größer 50 ist drückt der MOtor richtung 100
    digitalWrite(IN1, pwr2);
  }
  if (posi > 100) {
    analogWrite(IN2, pwr4);            //wenn Position größer als 100 fährt der Motor richtung 100
    digitalWrite(IN1, LOW);
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
