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

  void setDirectedThrottle(int thro, int base) const {
    setForward(thro >= 0);
    
    int val = thro==0?0:min(abs(thro)+base,255);
    setThrottle(val);
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
PDVars pd(1, 0);
TKGyro gyro(A2, A3, TK_X1);

long lastSampleTime = 0;
double setAngle = 0;
double angle = 0;
double lastRate = 0;
double output = 0;
long rawRate = 0;
int beginning = 0;
double zeroOff=510;
long offSum=0;

int i = 0;

void setup() {
  Serial.begin(115200);
  delay(100);
  lastSampleTime = millis();
  lastRate = 0.0;
  
  right.setThrottle(0);  
}

void loop() {
  long now = micros();
  long dt = now-lastSampleTime;
  if(dt >= 1000){
    lastSampleTime=now;
    rawRate = gyro.getYAxis();
    
    if(i<1000) {
      ++i;
      offSum += rawRate;
      zeroOff = 509;//(double)(offSum)/(double)i;
      angle = 0.0;
    } else {
      offSum += rawRate;
      offSum += (double)(rawRate)/double(i);
    }
 /*   if(i==100){
      for(int x=0 ; x<50 ; ++x)
        Serial.println("================ CALLIBRATED ===============");
      ++i;
    }
 */   
    double rate = ((rawRate-zeroOff)*14633.0)/1000.0;
  //  angle += rate*((double)dt)/1000000.0;
    angle += ((rate+lastRate)*(double)dt)/2000000.0;
    lastRate = rate;
      
    pd.computeErrors(setAngle, rate); 
    int thro = pd.computeOutput();

    Serial.print(zeroOff);
    Serial.print("     ");
    Serial.print(rawRate-zeroOff);
    Serial.print("     ");
    Serial.print(thro);
    Serial.print("     ");
    Serial.println(angle);
  
    right.setDirectedThrottle(thro,25);
   // int thro = (int)angle;
   // right.setForward(thro <= 0);
   // right.setThrottle(min(abs(thro)+50,255));
   
  }
//  right.setDirectedThrottle();
}


