#include <util/atomic.h> // For the ATOMIC_BLOCK macro
#define ENCA 2 // YELLOW
#define ENCB 3 // Green


const int IN1 = 8;
const int IN2 = 9;
bool dir;

volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/


void setup() {

  Serial.begin(9600);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);

  dir = true;
}



void loop() { 
//Read the position in an atomic block to avoid a potentialmisread if the interrupt coincides with this code running
int pos = 0;
//see: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    pos = posi;

    if (dir = true){
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      
    if (pos >= -2000 ){
      dir = false;
      }
      else if (dir = false){
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      }
    }
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