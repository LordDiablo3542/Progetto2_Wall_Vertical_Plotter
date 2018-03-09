1.  Creare un programma su Arduino IDE con questo codice:
const int dirD = 4;     //pin che indica la direzione per il motore di destra
const int stepD = 3;  // pin che serve per far girare il motore di destra
const int dirS = 6;     // pin che indica la direzione del motore di sinistra
const int stepS = 5;  // pin che serve per far girare il motore di sinistra
const int pinEnable = 8;   //pin per abilitare i decoder A4988
void setup() { 
  pinMode(dirD, OUTPUT);
  pinMode(stepD, OUTPUT);
  pinMode(dirS, OUTPUT);
  pinMode(stepS, OUTPUT);
  pinMode(pinEnable, OUTPUT);
   
  digitalWrite(pinEnable, LOW); //attivo i moduli A4988
}
  
void loop() {
  digitalWrite(dirD, HIGH);
  digitalWrite(dirS, HIGH);
  for(int i = 0; i<2400; i++){
     digitalWrite(stepD, HIGH);
     digitalWrite(stepS, HIGH);
     delayMicroseconds(300);
     digitalWrite(stepD, LOW);
     digitalWrite(stepS, LOW);
     delayMicroseconds(300);
  }
  
  delay(500);
  digitalWrite(dirD, LOW);
  digitalWrite(dirS, LOW);
   
  for(int i = 0; i<2400; i++){
     digitalWrite(stepD, HIGH);
     digitalWrite(stepS, HIGH);
     delayMicroseconds(300);
     digitalWrite(stepD, LOW);
     digitalWrite(stepS, LOW);
     delayMicroseconds(300);
  }
  delay(500); 
}

