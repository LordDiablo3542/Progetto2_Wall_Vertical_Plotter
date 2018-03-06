#include <SPI.h>
#include <SD.h>
#include <Servo.h>
#include <Stepper.h>

const int rightPin2 = 4;
const int rightPin1 = 3;
const int leftPin2 = 6;
const int leftPin1 = 5;
const int steps = 200;
const int servoPin = 7;

Stepper dStepper(steps, rightPin1, rightPin2);
Stepper sStepper(steps, leftPin1, leftPin2);


Servo servo;
const int chipSelect = 4;
File dataFile;

String readRow[1];
String t = "";
char buffer[100];
int idx = 0;
char c;
int rowG = 0;
int rowX = 0;
int rowY = 0;
int rowZ = 0;
int rowJ = 0;
int rowI = 0;
String stG;
String stX = "0";
String stY = "0";
String stZ;
String stJ;
String stI;
String newstX;
String newstY;

void setup() {

  Serial.begin(9600);
    
  //velocità dei motori
  dStepper.setSpeed(100);
  sStepper.setSpeed(100);

  //iniziallizare il pin
  servo.attach(servoPin);
  
  //inizializazione SD 
  // Controlla se la scheda SD si può inizializare o se é presente.

  //Apre il file
  if (!SD.begin(chipSelect))
  {
    Serial.println("scheda non leggibile o non presente");
    // Smette di continuare il programma
    while(1);
  }else {
    String fileName = "prova.mpt";
    dataFile = SD.open(fileName);  
  }
  while(1)
  {
    //readLine();
    readGCode(readLine());
    delay(10);
  }
  // Se il file non si può aprire stampa un errore.

}

void loop() {
}




char readCharSD()
{
  return dataFile.read();
}




String readLine()
{
  bool EOL = false;
  while(!EOL)
  {
    c = readCharSD();
    if(c == '\n')
    {
      buffer[idx] = 0;
      idx = 0;
      EOL = true;
    }
    else
    {
      buffer[idx] = c;
      idx++;
    }
  }

  return buffer;
}







void readGCode(String row)
{
  bool f = true;
  rowG = row.indexOf('G');
  rowX = row.indexOf('X');
  rowY = row.indexOf('Y');
  rowZ = row.indexOf('Z');
  rowI = row.indexOf('I');
  rowJ = row.indexOf('J');
  if(rowJ == -1){
    f=false;
    rowJ = row.length();
    rowI = row.length();
    stI = "";
    stJ = "";
  }
  stG = row.substring(rowG + 1, rowX - 1);
  newstX = row.substring(rowX + 1, rowY - 1);
  newstY = row.substring(rowY + 1, rowZ - 1);
  stZ = row.substring(rowZ + 1, rowI - 1);
  if(f){
    stI = row.substring(rowI + 1, rowJ - 1);
    stJ = row.substring(rowJ + 1, row.length() - 1);
  }
  Serial.println(stG);
  Serial.println(newstX);
  Serial.println(newstY);
  Serial.println(stZ);
  Serial.println(stI);
  Serial.println(stJ);
  moveMotor(newstX.toInt(), newstY.toInt(),0);
}


void moveMotor (int x, int y, bool needServo) {
  int  si= 0;
  double  di= 0;
  if(needServo){
    servo.write(20);
  }
  if(x == 0 || y == 0){
    for( int i = 0 ; i < max(abs(x),abs(y)) ; i++ ){
      if(y == 0){
        if(x > 0){
          sStepper.step(1);
          dStepper.step(1);
          si++;
          di++;
        }
        else if(x < 0){
          sStepper.step(-1);
          dStepper.step(-1);
          si--;
          //di--;
        }
      }
      else if(x == 0){
        if(y > 0){
          sStepper.step(1);
          dStepper.step(-1);
          si++;
          di--;
        }
        else if(y < 0){
          sStepper.step(-1);
          dStepper.step(1);
          si--;
          di++;
        }
      }
    }
  }
  else{
    for( int i = 0 ; i < min(abs(x),abs(y)) ; i++ ){
      if(x > 0 && y > 0){
        if(x == y){
          dStepper.step(1);
          //di++;
        }
        else if(x > y){
          dStepper.step(1);
          sStepper.step(1*( long(max(x,y))/long(min(x,y)) ));
          di += 1*( long(max(x,y))/long(min(x,y)) );
          si++;
        }
        else{
          dStepper.step(1);
          sStepper.step(1*( long(max(x,y))/long(min(x,y)) ));
          //di++;
          si += 1*( long(max(x,y))/long(min(x,y)) );
        }
      }
      else if(x < 0 && y > 0){
        if(x == y){
          sStepper.step(1);
          si++;
        }
        else if(x > y){
          sStepper.step(-1);
          dStepper.step(1*( long(max(x,y))/long(min(x,y)) ));
          //di--;
          si += 1*( long(max(x,y))/long(min(x,y)) );
        }
        else{
          sStepper.step(1);
          dStepper.step(1-1*( long(max(x,y))/long(min(x,y)) ));
          si++;
          di += -1*( long(max(x,y))/long(min(x,y)) );
        }
      }
      else if(x > 0 && y < 0){
        if(x == y){
          dStepper.step(-1);
          //di--;
        }
        else if(x > y){
          dStepper.step(1*( long(max(x,y))/long(min(x,y)) ));
          sStepper.step(-1);
          si--;
          di += 1*( long(max(x,y))/long(min(x,y)) );
        }
        else{
          dStepper.step(1);
          sStepper.step(-1*( long(max(x,y))/long(min(x,y)) ));
          si += -1*( long(max(x,y))/long(min(x,y)) );
          //di++;
        }
      }
      else if(x < 0 && y < 0){
        if(x == y){
          sStepper.step(-1);
          si--;
        }
        else if(x > y){
          dStepper.step(-1*( long(max(x,y))/long(min(x,y)) ));
          sStepper.step(-1);
          si--;
          di += -1*( long(max(x,y))/long(min(x,y)) );
        }
        else{
          dStepper.step(-1);
          sStepper.step(-1*( long(max(x,y))/long(min(x,y)) ));
          si += -1*( long(max(x,y))/long(min(x,y)) );
          //di--;
        }
      }
    }
  }
  Serial.print("si:");
  Serial.println(si);
  Serial.print("di:");
  Serial.println(di);
  stX = newstX;
  stY = newstY;
  servo.write(0);
}





