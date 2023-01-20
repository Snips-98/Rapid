#include <util/atomic.h> // For the ATOMIC_BLOCK macro
#define ENCA 2 
#define ENCB 3 
#define aktuellesjahr 2022

const int IN1 = 8;    
const int IN2 = 9;    
bool dir;             
int ri;
volatile int posi = 0;

//Deklaration Variablen zuende.


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

//see: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    pos = posi;
    //zu Beginn ist dir=True
    if (dir and pos>50){
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
        dir=false;               
      }
    
    if (!dir){                    
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      if(pos<-10){
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
      }
 
    
    
    } 
  }
  //Serial.println(pos);
  Serial.print(dir);   
  Serial.print("  ");  
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