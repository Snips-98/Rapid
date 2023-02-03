#include <util/atomic.h> // For the ATOMIC_BLOCK macro
#define ENCA 2 // 
#define ENCB 3 //
#define PWM 7

const int IN1 = 8;   
const int IN2 = 9;            
volatile int posi = 0; 
int count1 = 0;
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
  
  mod1 = posi % 10; //teilen der Position durch 10 und speichern des Rests in der Variablen mod1
  pwr = map(abs(pos), 0, 200, 220, 220); 

  if (mod1 == 0 && posi > count1) {           //wenn Rest 0 und in die Positive Richtung gedreht wird        
    digitalWrite(IN1, 255);              
    digitalWrite(IN2, LOW);             
  } 
  else if (mod1 == 0 && posi < count1) {      //wenn Rest 0 und in die negative Richtung gedreht wird
    digitalWrite(IN1, LOW);              
    digitalWrite(IN2, 255);
  }
  else {                                      //sonst Motor aus
    digitalWrite(IN1, LOW);            
    digitalWrite(IN2, LOW);
  }

  count1 = posi;

  Serial.println("position");
  Serial.println(pos);
  Serial.println();
  Serial.println("modulo");
  Serial.println(mod1);
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
