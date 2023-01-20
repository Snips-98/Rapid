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

  if (pos >= 50){
    setMotor(-1, 0.1, PWM, IN1, IN2);
  }

  if (pos <= 0){
    setMotor(0, 0, PWM, IN1, IN2);
  }

  
  Serial.println(pos);
}

void setMotor(int dir, int pwmVal, int pwm, int in1, int in2){
  pwmVal = map(posi, 0, 50, 0, 255);
  analogWrite(pwm,pwmVal);
  if(dir == 1){
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
  }
  else if(dir == -1){
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }
  else{
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
  }
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