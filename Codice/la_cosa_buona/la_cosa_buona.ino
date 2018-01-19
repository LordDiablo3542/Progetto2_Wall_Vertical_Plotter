-
const int dirD = 4;
const int stepD = 3;
const int dirS = 6;
const int stepS = 5;

const int pinEnable = 8;

void setup() {
  // put your setup code here, to run once: 
  pinMode(dirD, OUTPUT);
  pinMode(stepD, OUTPUT);
  pinMode(dirS, OUTPUT);
  pinMode(stepS, OUTPUT);
  pinMode(pinEnable, OUTPUT);
    
  digitalWrite(dirD, HIGH);
  digitalWrite(dirS, HIGH);
  digitalWrite(pinEnable, LOW);
}
  
void loop() {
  digitalWrite(dirD, HIGH);
  digitalWrite(dirS, HIGH);
  for(int i = 0; i<2400; i++){
     digitalWrite(stepD, HIGH);
     digitalWrite(stepS, HIGH);
     delayMicroseconds(700);
     digitalWrite(stepD, LOW);
     digitalWrite(stepS, LOW);
     delayMicroseconds(700);
  }
  
  delay(500);
  digitalWrite(dirD, LOW);
  digitalWrite(dirS, LOW);
   
  for(int i = 0; i<2400; i++){
     digitalWrite(stepD, HIGH);
     digitalWrite(stepS, HIGH);
     delayMicroseconds(700);
     digitalWrite(stepD, LOW);
     digitalWrite(stepS, LOW);
     delayMicroseconds(700);
  }
  delay(500); 
}

//x: coordinata x in qui disegnare
//y: coordinata y in qui disegnare
//f: velocita con la quale muovere i motori
//e: attaccare/staccare pennarello dalla superficie
//void disegna(int x, int y, int f, bool e){
  

  
//}








