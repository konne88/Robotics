#include <TinkerKit.h>

class Motor {
public:
  Motor(int pinMagnitude, int pinForward) 
  : pinMag(pinMagnitude),
    pinForw(pinForward)
  {
     pinMode(pinMag,OUTPUT);
     pinMode(pinForw,OUTPUT);
  }

  void set(int mag, boolean forw) const {
    digitalWrite(pinForw, forw?HIGH:LOW);
    analogWrite(pinMag, mag);
  }
  
private:
  const int pinMag;
  const int pinForw;
};

const Motor left(11,13);
const Motor right(3,12);
TKAccelerometer accel(A2, A3);

void setup() {
  Serial.begin(9600);
  delay(2000);
} 

void loop() {
  int target = 503;
  
  long xAxisValue = accel.getXAxis();  
  long yAxisValue = accel.getYAxis(); 
  bool dir = xAxisValue > target;

  Serial.print(millis());
  Serial.print(',');
  Serial.print(xAxisValue);
  Serial.print(',');
  Serial.print(yAxisValue);
  Serial.print(',');
  Serial.println(dir);
  
  left.set(255,dir);
  right.set(255,dir);
  
  delay(10);
}

