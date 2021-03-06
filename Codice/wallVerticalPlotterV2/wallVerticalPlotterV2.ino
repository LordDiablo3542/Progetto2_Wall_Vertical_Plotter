#include <SPI.h>
#include <SD.h>
#include <Servo.h>
#include <Stepper.h>

const int dPin2 = 4;
const int dPin1 = 3;
const int sPin2 = 6;
const int sPin1 = 5;
const int steps = 200;

Stepper dStepper(steps, dPin1, dPin2);
Stepper sStepper(steps, sPin1, sPin2);

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
bool isRelativ = true;
int actualX = 0;
int actualY = 0;

void readGCode (String s){
  Serial.println("enter gcode...");
  for(int i = 0 ; i <= s.length() ; i++){
    if(s.charAt(i) == 'G'){
    	isG = true;
    	if(s.charAt(i+1) == '9'){
    		//i++;
    		if(s.charAt(i+2) == '0'){
    			//i++;
    			isRelativ = false;
    		}
    		else if(s.charAt(i+2) == '1'){
    			//i++;
    			isRelativ = true;
    		}
    		else if(s.charAt(i+2) == '2'){
    			//i++;
    			isMove = true;
    		}
    	}
    }
    else if( (s.charAt(i) == 'X' || s.charAt(i) == 'x')  && isG){
		isE = false;
    	isY = false;
    	isX = true;
    }
    else if((s.charAt(i) == 'Y' || s.charAt(i) == 'y')  && isG){
		isX = false;
		isE = false;
    	isY = true;
    }
    else if((s.charAt(i) == 'Z' || s.charAt(i) == 'z')  && isG){
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
  			moveMotor(corX.toInt()-actualX, corY.toInt()-actualY, false);
  			actualX = corX.toInt();
  			actualY = corY.toInt();
  		}
  		else{
  			paint(corX.toInt(), corY.toInt(), corE.toInt());
    	}
      Serial.println("X:"+corX + " Y:" + corY + " E:" + corE);
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
  int i = sqrt(pow(x, 2) + pow(y, 2))
  double r = 1;
  if(i < e){
    double r = e/i;
  }
    
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

void moveMotor (int x, int y, bool needServo) {
  if(needServo){
    servo.write(20);
  }
  if(x == 0 || y == 0){
    for( int i = 0 ; i < max(abs(x),abs(y)) ; i++ ){
      if(y == 0){
        if(x > 0){
          sStepper.step(1);
          dStepper.step(1);
        }
        else if(x < 0){
          sStepper.step(-1);
          dStepper.step(-1);
        }
      }
      else if(x == 0){
        if(y > 0){
          sStepper.step(1);
          dStepper.step(-1);
        }
        else if(y < 0){
          sStepper.step(-1);
          dStepper.step(1);
        }
      }
    }
  }
  else{
    for( int i = 0 ; i < min(abs(x),abs(y)) ; i++ ){
      if(x > 0 && y > 0){
        if(x == y){
          dStepper.step(1);
        }
        else if(x > y){
          dStepper.step(1*( long(max(x,y))/long(min(x,y)) ));
          sStepper.step(1);
        }
        else{
          dStepper.step(1);
          sStepper.step(1*( long(max(x,y))/long(min(x,y)) ));
        }
      }
      else if(x < 0 && y > 0){
        if(x == y){
          sStepper.step(1);
        }
        else if(x > y){
          sStepper.step(1*( long(max(x,y))/long(min(x,y)) ));
          dStepper.step(-1);
        }
        else{
          sStepper.step(1);
          dStepper.step(-1*( long(max(x,y))/long(min(x,y)) ));
        }
      }
      else if(x > 0 && y < 0){
        if(x == y){
          dStepper.step(-1);
        }
        else if(x > y){
          dStepper.step(1*( long(max(x,y))/long(min(x,y)) ));
          sStepper.step(-1);
        }
        else{
          dStepper.step(1);
          sStepper.step(-1*( long(max(x,y))/long(min(x,y)) ));
        }
      }
      else if(x < 0 && y < 0){
        if(x == y){
          sStepper.step(-1);
        }
        else if(x > y){
          dStepper.step(-1*( long(max(x,y))/long(min(x,y)) ));
          sStepper.step(-1);
        }
        else{
          dStepper.step(-1);
          sStepper.step(-1*( long(max(x,y))/long(min(x,y)) ));
        }
      }
    }
  }
  servo.write(0);
}

void setup() {
  //motor
  dStepper.setSpeed(100);
  sStepper.setSpeed(100);

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
    //while (1);
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
  delay(1000);

}

void loop() {
}
