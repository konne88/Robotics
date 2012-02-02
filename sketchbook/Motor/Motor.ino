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
TKAccelerometer accel(A2, A3);

const int target = 0;

byte done[SENSOR_RANGE][MOTOR_RANGE/8];

void setup() {
  Serial.begin(115200);
  
  for(int x=0 ; x<SENSOR_RANGE; ++x) {
    for(int y=0 ; y<MOTOR_RANGE/8 ; ++y) {
      done[x][y] = 0;
    }
  }
  
  Serial.print("# Throttle index, Angle Index, Throttle, Angle, Result Angle, Result Error");
 
} 

int getAccelerometerAngleInDegrees() {
  long x = accel.getXAxis() - 473; 
  long y = accel.getYAxis() - 480;
  return atan2(x,y)/3.14159265*180.0;
}

void loop() {
  // angles from about +19 -> - 20
  // mapped to 0-19
  int angle = getAccelerometerAngleInDegrees();
  int xi = (min(max(angle,-20),19)+20)/4;
  
  int i;
  int b;
  
  int throttle;
  bool record = true;

  // first fill the matrix sparsely
  for(i=0 ; i<(MOTOR_RANGE/8) ; ++i) {
    if(done[xi][i] == 0) {
      done[xi][i] = 1;
      throttle = ((i*8)*(512/MOTOR_RANGE))-255;      
      goto out;
    }
  }  
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
  
  // empty the row
  left.setDirectedThrottle(0);
  right.setDirectedThrottle(0);
  
  delay(1000);
  record = false;
  throttle = angle>target?255:-255;
  
  out:
  
  // move
  left.setDirectedThrottle(throttle);
  right.setDirectedThrottle(throttle);
  delay(100);

  // wait for the movement's impact
  if(record){
    Serial.print(i*8+b);
    Serial.print(',');
    Serial.print(xi);
    Serial.print(',');
    Serial.print(throttle);
    Serial.print(',');
    Serial.print(angle);
    Serial.print(',');
    Serial.print(getAccelerometerAngleInDegrees());
    Serial.print(',');
    Serial.println(abs(target - getAccelerometerAngleInDegrees()));
  }
}


