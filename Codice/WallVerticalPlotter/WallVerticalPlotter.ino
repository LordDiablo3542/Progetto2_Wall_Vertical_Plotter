#include <SPI.h>
#include <SD.h>
#include <Servo.h>

const int dirD = 4;
const int stepD = 3;
const int dirS = 6;
const int stepS = 5;
const int pinEnable = 8;

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
    		s.charAt(i) == '6' || s.charAt(i) == '7' || s.charAt(i) == '8' || s.charAt(i) == '9' || s.charAt(i) == '-') && isG){
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
  		if(isMove){
  			//moveMotor(corX.toInt()-actualX, corY.toInt()-actualY, false);
  			actualX = corX.toInt();
  			actualY = corY.toInt();
  		}
  		else{
  			//paint(corX.toInt(), corY.toInt(), corE.toInt());
    	}
      Serial.println("X:"+corX + " Y:" + corY + " E:" + corE + " act. X:" + actualX + " act. Y:" + actualY);
  		isX = false;
  		isY = false;
  		isE = false;
  		isMove = false;
  		isG = false;
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
		servo.write(60);
	}
	else{
		servo.write(0);
	}

	if(x < 0){
	  	digitalWrite(dirS, HIGH);
	}
	else if(x > 0){
	  	digitalWrite(dirS, LOW);
	}
	else if(x == 0){
		if(y < 0){
		  	digitalWrite(dirS, LOW);
		}
		else if(y > 0){
		  	digitalWrite(dirS, HIGH);
		}
	}

	if(y < 0){
	  	digitalWrite(dirD, HIGH);
	}
	else if(y > 0){
	  	digitalWrite(dirD, LOW);
	}
	else if(y == 0){
		if(x < 0){
	  		digitalWrite(dirD, HIGH);
		}
		else if(x > 0){
		  	digitalWrite(dirD, LOW);
		}
	}

	for(int i = 0; i < max(x, y); i++){
	    digitalWrite(stepD, HIGH);
	    digitalWrite(stepS, HIGH);
	    delayMicroseconds(700);
	    digitalWrite(stepD, LOW);
	    digitalWrite(stepS, LOW);
	    delayMicroseconds(700);
	}
}

void setup() {
  //motor
  pinMode(dirD, OUTPUT);
  pinMode(stepD, OUTPUT);
  pinMode(dirS, OUTPUT);
  pinMode(stepS, OUTPUT);
  pinMode(pinEnable, OUTPUT);

  //servo
  servo.attach(servoPin);
  servo.write(0);
  
 

  //SD 
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  Serial.print("Inizializazione scheda SD...");

  // Controlla se la scheda si può inizializare o se é presente.
  if (!SD.begin(chipSelect)) {
    Serial.println("scheda non leggibile o non presente");
    // Non fà più niente.
    while (1);
  }
  Serial.println("scheda inizializata.");

  //Apre il file
  String fileName = "prova.mpt";
  dataFile = SD.open(fileName);

  // Se avviabile, comincia a leggerlo.
  if (dataFile) {
    String s = "";
    while (dataFile.available()) {
      t = dataFile.read();
      s += char(t);
    }
    //Passa il contenuto al metodo e chiude il file.
    readGCode(s);
    dataFile.close();
  }
  // Se il file non si può aprire stampa un errore.
  else {
    Serial.println("error opening " + fileName );
  }
  
}

void loop() {

}
