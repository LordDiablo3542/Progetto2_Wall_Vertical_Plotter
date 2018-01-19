#include <SPI.h>
#include <SD.h>
#include <Servo.h>

int servoPin = 7;
Servo servo;
const int chipSelect = 4;
File dataFile;
int t;
bool isX = false;
bool isY = false;
bool isE = false;
bool isMove = false;
bool isG = false;
String corX = "";
String corY = "";
String corE = "";
bool isRelativ = false;
int actualX = 0;
int actualY = 0;

void readGCode (String s){
  for(int i = 0 ; i < s.length() ; i++){
    if(s.charAt(i) == 'G'){
    	isG = true;
    	if(s.charAt(i+1) == '9'){
    		i++;
    		if(s.charAt(i+1) == '0'){
    			i++;
    			isRelativ = false;
    		}
    		else if(s.charAt(i+1) == '1'){
    			i++;
    			isRelativ = true;
    		}
    		else if(s.charAt(i+1) == '2'){
    			i++;
    			isMove = true;
    		}
    	}
    }
    else if(s.charAt(i) == 'X'  && isG){
		isE = false;
    	isY = false;
    	isX = true;
    }
    else if(s.charAt(i) == 'Y'  && isG){
		isX = false;
		isE = false;
    	isY = true;
    }
    else if(s.charAt(i) == 'E'  && isG){
    	isY = false;
    	isX = false;
    	isE = true;
    }
    else if((s.charAt(i) == '0' || s.charAt(i) == '1' || s.charAt(i) == '2' || s.charAt(i) == '3' || s.charAt(i) == '4' || s.charAt(i) == '5' ||
    		s.charAt(i) == '6' || s.charAt(i) == '7' || s.charAt(i) == '8' || s.charAt(i) == '9') && isG){
    	if(isY){
    		corY += s.charAt(i);
    	}
    	else if(isX){
    		corX += s.charAt(i);
    	}
    	else if(isE){
    		corE += s.charAt(i);
    	}
    }
    else if(s.charAt(i) == ';'){
  		isX = false;
  		isY = false;
  		isE = false;
  		isMove = false;
  		isG = false;
  		if(isMove){
  			//moveMotor(corX.toInt()-actualX, corY.toInt()-actualY, false);
  			actualX = corX.toInt();
  			actualY = corY.toInt();
  		}
  		else{
  			//paint(corX.toInt(), corY.toInt(), corE.toInt());
    	}
      Serial.println("X:"+corX + " Y:" + corY + " E:" + corE + " act. X:" + actualX + " act. Y:" + actualY);
      corX = "";
    	corY = "";
    	corE = "";
    }
    else{
  		isX = false;
  		isY = false;
  		isE = false;
    }
  }
}

void paint (int x, int y, int e){
	if (isRelativ){
		moveMotor(x, y, true);

		actualX += x;
		actualY += y;
	}
	else{
		moveMotor(x-actualX, y-actualY, true);

		actualX += x-actualX;
		actualY += y-actualY;
	}
}

void moveMotor (int x, int y, bool s) {
	if(s){
		servo.write(-10);
	}
	else{
		servo.write(10);
	}
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  //servo
  servo.attach(servoPin);
  servo.write(-10);
  
  //SD
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  dataFile = SD.open("prova.txt");

  // if the file is available, write to it:
  if (dataFile) {
    String s = "";
    while (dataFile.available()) {
      t = dataFile.read();
      s += char(t);
    }
    Serial.println(s);
    readGCode(s);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening prova.txt");
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
