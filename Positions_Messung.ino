#include <util/atomic.h> // For the ATOMIC_BLOCK macro
#include <Stepper.h> //stepper motor

#define ENCA 2 // YELLOW dc
#define ENCB 3 // WHITE dc
#define PWM 7 // PURPLE step
#define IN2 9 // BLUE step
#define IN1 8 // YELLOW2 step

volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/ loads position from RAM



//Stepper stepper(STEPS, 8, 9, 10, 11);
//const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
//const int rolePerMinute = 17;         // Adjustable range of 28BYJ-48 stepper is 0~17 rpm

void setup() {
  Serial.begin(9600);
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING); //rising = trigger when pin goes from low to high
  
  pinMode(PWM,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);

}



void loop() {
    int pos = 0;
  // Read the position in an atomic block to avoid a potential
  // misread if the interrupt coincides with this code running
  // see: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    pos = posi;
  if (pos < 30) {
    setMotor(1, 100, PWM);
  }
  else if (pos < 60) {
    setMotor(-1, 100, PWM);
  }
  setMotor(0, 25, PWM);
  }
    Serial.println(pos);
}

void setMotor(int dir, int pwmVal, int pwm){
  //analogWrite(pwm,pwmVal);
  if(dir == 1){
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
  }
  else if(dir == -1){
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
  }
  else{
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
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