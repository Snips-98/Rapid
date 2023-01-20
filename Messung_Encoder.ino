#define ENCA 2  // Yellow
#define ENCB 3  // Green
#define IN2 9   // BLUE step
#define IN1 8   // YELLOW2 step


void setup() {
  Serial.begin(9600);
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void loop() {
  int a = digitalRead(ENCA);
  int b = digitalRead(ENCB);
  //igitalWrite(IN1, LOW);
  //digitalWrite(IN2, HIGH);
  Serial.print(a * 5);
  Serial.print(" ");
  Serial.print(b * 5);
  Serial.println();
  //delay(1000);
}