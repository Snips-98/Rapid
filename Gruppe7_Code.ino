#include <util/atomic.h> // For the ATOMIC_BLOCK macro
#define ENCA 2 //Auslesen des Motors
#define ENCB 3 //Auslesen des Motors
#define PWM 7 

//Motor Treiber Anschluss
const int INA = 13; //PIN M2A
const int INB = 12; //PIN M2B
const int IN1 = 8;   
const int IN2 = 9;            
//Power für den Motor
long pwr;
long pwr2;
long pwr3;
long pwr4;
//Variablen
int mod1 = 0;
int caseNumber;
int count1 = 0;
volatile int posi = 0; //Wert wird aus dem Arbeitsspeicher gelesen, ist nötig da mit Interrups gearbeitet wird.


void setup() {  

  Serial.begin(9600);   
  pinMode (IN1, OUTPUT);   
  pinMode (IN2, OUTPUT);
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  pinMode(INA, INPUT);
  pinMode(INB, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);   
  caseNumber = 1;
}


void loop() {

  //pos = posi ist atom das immer als Block ausgeführt wird und nicht von interrups beeinflusst wird (atomic_block)
  int pos = 0;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    pos = posi;
  }

  //Auslesen des Bottons M2A am Motortreiber. Beim drücken des Buttons wird der case erhöht
  int buttonStateINA = digitalRead(INA);
  if (buttonStateINA == HIGH) {
    caseNumber ++;
    delay(600);
  }
  if (caseNumber > 4) {
    caseNumber = 1;
  }

  //Auslesen des Bottons M2B am Motortreiber. Beim drücken des Buttons wird die Position zurückgesetzt
  int buttonStateINB = digitalRead(INB);
  if (buttonStateINB == HIGH) {
    posi = 0;
  }

  //Switch funktion mit 4 cases hier kann zwischen den einzelnen Programmen gewechselt werden
  switch (caseNumber) {

    //case gibt nur die aktuelle Position des Motors aus. Ist die Startfunktion.
    case 1:
      Serial.println("Aktuelle Position");
      Serial.println(pos);
      break;

    //case das der Joystick immer zur Position 0 zurück kehrt
    case 2:
      Serial.println("0 - 1 - 0");
      pwr = map(abs(pos), 0, 80, 55, 180); //wenn der Hebel sich der Position 0 nähert soll er auf die Leistung 55 schalten, wenn er sich der Position 80 nähert soll er bis zu diesem Punkt mit der Leistung linear ansteigen bis zum Leistungswert 180

      if (posi < -1) {                      //wenn Position kleiner als -1 also -2,-3.... 
        analogWrite(IN1, pwr);              //...dann gilt für IN1 die Befehle der Map Funktion 
        digitalWrite(IN2, LOW);             //...in andere Richtung ist aus  
      } 
      else if (posi > 1) {                  //wenn Position größer als 1.... 
        analogWrite(IN2, pwr);              //...dann gilt für IN1 die Befehle der Map Funktion
        digitalWrite(IN1, LOW);             //...in andere Richtung ist aus
      } 
      else {                                //wenn Poition 0 dann ist der motor aus
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
      }
  
      Serial.println(pos);
      break;

    //case das der Motor von der pos 0-50 in richtung 0 drückt und ab 50 in richtung 100 das gleiche in die andere Richtung
    case 3:
      Serial.println("0 - 1 snap");
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
      break;

    //case das der Motor immer in 10er Schritten dagegen hält also immer wenn die Position durch 10 keinen Rest ergibt schaltet der Motor an
      Serial.println("10 steps");
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
      break;
  }
}

//Auslesen der Position des Motors über M1B des Motors
void readEncoder() {                    
  int b = digitalRead(ENCB);
  if(b > 0){
    posi++;
  }
  else{
    posi--;
  }
}

