#include <util/atomic.h> // For the ATOMIC_BLOCK macro

#define ENCA 2 // YELLOW
#define ENCB 3 // GREEN
#define PWM 7 //5 ORANGE
#define IN2 8 //6 PURPLE
#define IN1 9 //7 BLACK

volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/

void setup() {
  Serial.begin(9600);
  pinMode(ENCA,INPUT); //yellow
  pinMode(ENCB,INPUT); //white
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);
  
  pinMode(PWM,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
}

void loop() {
  
  // Read the position in an atomic block to avoid a potential
  // misread if the interrupt coincides with this code running
  // see: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
  int pos = 0; 
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    pos = posi;
  }
  if (pos < 512) {
    setMotor(1, 25, PWM, IN1, IN2);
  }
  else if(pos > 512){
    setMotor(-1, 25, PWM, IN1, IN2);
  }
  else if(pos > 1024){
    setMotor(1, 25, PWM, IN1, IN2);
  }
  /*setMotor(1, 25, PWM, IN1, IN2);
  delay(200);
  Serial.println(pos);
  setMotor(-1, 25, PWM, IN1, IN2);
  delay(200);
  Serial.println(pos);
  setMotor(0, 25, PWM, IN1, IN2);
  delay(200);*/
  Serial.println(pos); 
}

void setMotor(int dir, int pwmVal, int pwm, int in1, int in2){
  analogWrite(pwm,pwmVal);
  if(dir == 1){
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
  }
  else if(dir == -1){
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }
  else if(dir == 0){
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
  }
}

/*void readEncoderB(){
  int b = digitalRead(ENCB);
  if(b > 0){
    posi++;
  }
  else{
    posi--;
  }
}*/
void readEncoder(){
  int b = digitalRead(ENCB);
  if(b > 0){
    posi++;
  }
  else{
    posi--;
  }
}