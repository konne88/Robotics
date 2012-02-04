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

class PDVars {
  public:
    PDVars(double Kpp, double Kdd)
    : Kp(Kpp), Kd(Kdd), error(0), dError(0), lastError(0)
    {}
    
    void computeErrors(double setPoint, double input){
      error = setPoint - input;
      dError = error - lastError;
      lastError = error;
    }
    
    double computeOutput(){
      return Kp*error + Kd*dError;
    }
  
    double Kp;
    double Kd;
    double error;
    double dError;
    double lastError;
};


const Motor left(11,13);
const Motor right(3,12);
PDVars pd(1, 0.4);
TKGyro gyro(A2, A3, TK_X1);

long lastSampleTime = 0;
double setAngle = 0;
double angle = 0;
double lastRate = 0;
double output = 0;
long rawRate = 0;
int beginning = 0;
double zeroOff=510;
double offSum=0;

void setup() {
  Serial.begin(115200);
  delay(100);
  lastSampleTime = millis();
  lastRate = gyro.getYAxisRate();
}

void loop() {
  long now = millis();
  long dt = now-lastSampleTime;
  if(dt >= 10){
    rawRate = gyro.getYAxis();
    double rate = ((rawRate-zeroOff)*14633.0)/1000.0;
    angle += ((rate+lastRate)*(double)dt)/2000.0;
    
    lastRate = rate;
    
    pd.computeErrors(setAngle, angle); 
    output = pd.computeOutput();
    lastSampleTime=now;
  }
  Serial.println(rawRate);
  //right.setDirectedThrottle((int)output);
}


