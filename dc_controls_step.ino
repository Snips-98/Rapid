#include <util/atomic.h> // For the ATOMIC_BLOCK macro
#include <Stepper.h> //stepper motor

//#define STEPS 2048
#define ENCA 2 // YELLOW dc
#define ENCB 3 // WHITE dc
#define PWM 5 // PURPLE step
#define IN2 6 // BLUE step
#define IN1 7 // YELLOW2 step

//volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/ loads position from RAM
//Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
Stepper stepper(stepsPerRevolution, 8, 9, 10, 11);
//const int rolePerMinute = 17;         // Adjustable range of 28BYJ-48 stepper is 0~17 rpm

int pos = 0;
int b = digitalRead(ENCB);
int a = digitalRead(ENCA);

  //Stepper stepper(STEPS, 8, 9, 10, 11);

void setup() {
  Serial.begin(9600);
  stepper.setSpeed(200);
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCB),readEncoder,RISING); //rising = trigger when pin goes from low to high // reads ENCA // puts the value in the function readEncoder
  
}

void loop() {
  Serial.println(b);
  Serial.println(a); 

  //stepper.step(pos);
  delay(100);
}

void readEncoder(){
  
  if(b!=0){
    pos++;
    //stepper.step(1);
  }
  //else{
    //pos--;
  //stepper.step(pos);*/

}
