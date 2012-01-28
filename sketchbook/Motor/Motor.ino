#include <TinkerKit.h>

#define DEGREE 3
#define COLUMNS 2
#define DIMENSIONS DEGREE*COLUMNS

class Motor {
public:
  Motor(int pinThrottle, int pinForward) 
  : pinThro(pinThrottle),
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
TKAccelerometer accel(A2, A3);

int target;

void setup() {
  Serial.begin(115200);
  delay(1800);
  target = accel.getXAxis();
  delay(200);
} 

int theta[DIMENSIONS];
const int alpha = 1;

/**
 * calculates how much we thing that we hate the given x
 */
int hate(int x[]) {
  long sum = 0;
  for(int i=0 ; i<DIMENSIONS ; ++i) {
    sum += x[i]*theta[i];
  }
  return sum/DIMENSIONS;
}

void generate_powers(int x[][DEGREE]) {
  for(int c=0 ; c<COLUMNS ; ++c) {
    for(int d=1 ; d<DEGREE ; ++d) {
      x[c][d] = x[c][d-1]*x[c][0];
    }
  }
}

void loop() {
  int x[COLUMNS][DEGREE];
  int* px = (int*)x;
  int y;
  
  long start = millis();

  // find best throttle for our acceleration
  int bestThrottle = 0;
  int minHate = 10000;

  x[0][0] = accel.getXAxis();
  for(int i=-255 ; i<=255 ; ++i) {
    x[1][0] = i;
    generate_powers(x);
    if(hate(px) < minHate) {
      bestThrottle = i;
    }
  }
  
  // move
  left.setDirectedThrottle(bestThrottle);
  right.setDirectedThrottle(bestThrottle);

  // wait for the movement's impact
  delay(100);
  
  // update polynome
  x[1][0] = bestThrottle;
  generate_powers(x);
  y = abs(accel.getXAxis()-target);
  int error = hate(px)-y;
  for(int i=0 ; i<DIMENSIONS ; ++i) {
    theta[i] -= alpha*error*px[i];
  }

/*  for(int i=0 ; i<DIMENSIONS ; ++i) {
    Serial.print(theta[i]);
    Serial.print(',');
  }*/

  Serial.print(x[0][0]);
  Serial.print(',');
  Serial.print(x[1][0]);
  Serial.print(',');
  Serial.print(y);
  Serial.print(',');
  Serial.println(millis());

/*
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
  
  delay(10);*/
}

