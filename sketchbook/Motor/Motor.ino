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
  
  left.setForward(false);
  right.setForward(false);
  
  left.setThrottle(255);
  right.setThrottle(255);
  
  // 10 umdrehungen
  delay(8790);
  
  
  
 /* left.setForward(true);
  right.setForward(true);
  
  
  delay(1000);
  */
  
//  gyro.calibrate();

  left.setThrottle(0);
  right.setThrottle(0);
 
  lastSampleTime = micros();
}

void loop() {
  
}


