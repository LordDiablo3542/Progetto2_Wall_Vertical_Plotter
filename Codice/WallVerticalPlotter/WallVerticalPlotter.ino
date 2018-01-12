#include <SPI.h>
#include "FishinoSdFat.h"
SdFat SD;

float x;
float y;
bool isRelativ = false;
void readGCode (int text){
  while(){
    if(txt.equals("G1")){
      while(!txt.equals("/n")){
        if(txt.equals("x")){
          String sx = "";
          while(!txt.equals(" ")){
            sx.concat(txt);
          }
        }
        else if (txt.equals("y")) {
          String sy = "";
          while(!txt.equals(" ")){
            sy.concat(txt);
          }
        }
      }
      if(isRelativ){
        x += float(sx);
        y += float(sy);
      }
      else{
        x = float(sx);
        y = float(sy);
      }
    }
    else if(equals("G90")){
      isRelativ = false;
    }
    else if(equals("G91")){
      isRelativ = true;
    }
  }
}



void moveMotor (int cord){
  //muoviti motore!
}

void setup() {
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  if (SD.exists("example.txt")) {
    Serial.println("example.txt exists.");
  } else {
    Serial.println("example.txt doesn't exist.");
  }
  
  File file = SD.open("./prova.svg", FILE_READ);
  String text = String(char(file.read()));
  
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
