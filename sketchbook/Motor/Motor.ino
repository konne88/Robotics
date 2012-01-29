#include <TinkerKit.h>

#define DEGREE 3
#define COLUMNS 2
#define DIMENSIONS DEGREE*COLUMNS

#define MOTOR_RANGE 32
#define SENSOR_RANGE 20

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
TKAccelerometer accel(A2, A3);

int target;

byte done[SENSOR_RANGE][MOTOR_RANGE/8];

void setup() {
  Serial.begin(115200);
  //delay(1800);
  target = accel.getXAxis();
 // delay(200);
  
  for(int x=0 ; x<SENSOR_RANGE; ++x) {
    for(int y=0 ; y<MOTOR_RANGE/8 ; ++y) {
      done[x][y] = 0;
    }
  }
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
/*  int x[COLUMNS][DEGREE];
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
*/
  /*  for(int i=0 ; i<DIMENSIONS ; ++i) {
   Serial.print(theta[i]);
   Serial.print(',');
   }*/


  long x = accel.getXAxis(); 
  x = min(max(x,400),599);
  
  long xi = (x-400)*SENSOR_RANGE/200;
  
  int i;
  int b;
  
  int throttle;
  
  // find empty bit
  for(i=0 ; i<(MOTOR_RANGE/8) ; ++i) {
    if(done[xi][i] == 0xff) {
      continue;
    }
    for(b=0 ; b<8 ; ++b) {
      if((done[xi][i] >> b) & 1) {
         continue;
      }
      done[xi][i] |= (1<<b);
      throttle = ((i*8+b)*(512/MOTOR_RANGE))-255;
      goto out;
    }
  }
  
  throttle = 255;
  
  out:
  
  
  // move
  left.setDirectedThrottle(throttle);
  right.setDirectedThrottle(throttle);

  // wait for the movement's impact
  delay(30);
  Serial.print(i);
  Serial.print(',');
  Serial.print(b);
  Serial.print(',');
  Serial.print(xi);
  Serial.print(',');
  Serial.print(x);
  Serial.print(',');
  Serial.print(throttle);
  Serial.print(',');
  Serial.println(accel.getXAxis());

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


