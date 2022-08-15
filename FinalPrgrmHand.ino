#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#define numOfValsRec 5
#define digitsPerValRec 1
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  150
#define SERVOMAX  600

int ValsRec[numOfValsRec];
int stringLength = numOfValsRec * digitsPerValRec + 1; //$00000
int counter = 0;
bool counterStart = false;
String receivedString;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);

}

void receiveData() {
  while (Serial.available())
  {
    char c = Serial.read();
    if (c == '$') {
      counterStart = true;
    }
    if (counterStart) {
      if (counter < stringLength) {
        receivedString = String(receivedString + c);
        counter ++;
      }
      if (counter >= stringLength) {
        //$00000
        for (int i = 0; i < numOfValsRec; i++) {
          int num = (i * digitsPerValRec) + 1;
          ValsRec[i] = receivedString.substring(num, num + digitsPerValRec).toInt();
        }
        receivedString = "";
        counter = 0;
        counterStart = false;
      }
    }

  }
}

void loop() {
  int open = 180;
  int close = 0;
  int plseopen = map(open,0,180,SERVOMIN,SERVOMAX);
  int plseclose = map(close,0,180,SERVOMIN,SERVOMAX);
    
  int thumb = 3;
  int index = 4;
  int middle = 1;
  int ring = 0;
  int pinky = 2;
  
  receiveData();
    if (ValsRec[0] == 1){pwm.setPWM(thumb,0,plseclose);}else{pwm.setPWM(thumb,0,plseopen);}
    if (ValsRec[1] == 1){pwm.setPWM(index,0,plseclose);}else{pwm.setPWM(index,0,plseopen);}
    if (ValsRec[2] == 1){pwm.setPWM(middle,0,plseclose);}else{pwm.setPWM(middle,0,plseopen);}
    if (ValsRec[3] == 1){pwm.setPWM(ring,0,plseclose);}else{pwm.setPWM(ring,0,plseopen);}
    if (ValsRec[4] == 1){pwm.setPWM(pinky,0,plseopen);}else{pwm.setPWM(pinky,0,plseclose);}
}
