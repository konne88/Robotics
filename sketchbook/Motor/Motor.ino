#include <TinkerKit.h>

#define MOTOR_RANGE 16
#define SENSOR_RANGE 10

class Motor {
public:
  Motor(int pinThrottle, int pinForward) 
: 
    pinThro(pinThrottle),
    pinForw(pinForward)
    {
      pinMode(pinThro,OUTPUT);
      pinMode(pinForw,OUTPUT);
    }

  void setThrottle(byte thro) const {
    analogWrite(pinThro, thro);
  }

  void setForward(boolean forw) const {
    digitalWrite(pinForw, forw?HIGH:LOW);
  }

  void setDirectedThrottle(int thro) const {
    setForward(thro >= 0);
    setThrottle(min(abs(thro),255));
  }

private:
  const byte pinThro;
  const byte pinForw;
};

const Motor left(11,13);
const Motor right(3,12);
TKGyro gyro(A2, A3, TK_X1);

long lastSampleTime = 0;

void setup() {
  Serial.begin(115200);
  
 // left.setThrottle(255);
 // right.setThrottle(255);
  
  delay(1000);
  
//  gyro.calibrate();

//  left.setThrottle(0);
//  right.setThrottle(0);
 
  
  
  
  
  lastSampleTime = micros();
}

float angle = 0;

long  _xZeroVoltage = 0.0;
long  _yZeroVoltage = 0.0; 
int i = 0;
float xOff = 0;
float yOff = 0;

void loop() {
  if(i<50){
     ++i;
     _yZeroVoltage += gyro.getYAxis();
     _xZeroVoltage += gyro.getXAxis();
     
     xOff = (float)_xZeroVoltage/((float)i);
     yOff = (float)_yZeroVoltage/((float)i);
  }
  
  float change = (float)gyro.getYAxis() - yOff;
  long now = micros();
  float slice = (float)change*((float)(now-lastSampleTime)) / 14633.0;

  angle += slice;
  lastSampleTime = now;
  
  bool dir = angle<=0;
  Serial.print(dir?'<':'>');
  Serial.print("       ");
  Serial.print(change);
  Serial.print("       ");
  Serial.print(slice);
  Serial.print("       ");
  Serial.print(yOff);
  Serial.print("       ");
  Serial.println(angle);
  
  // empty the row
  //left.setForward(dir);
  //right.setForward(dir);
  int m = abs(angle)/50+50;
  
  left.setThrottle(m);
  right.setThrottle(m);
  left.setForward(dir);
  right.setForward(dir);
  
  delay(100);
}


