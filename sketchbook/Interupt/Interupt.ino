#include <TinkerKit.h>

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


#define ledPin 13

const Motor left(11,13);
const Motor right(3,12);
TKGyro gyro(I3, I3, TK_X1);
TKAccelerometer accelerometer(I4, I5);

int zeroOff = 509;
int xZero=488;
int yZero=485;

volatile long rawAngle;
volatile long samples;

volatile bool tog = false;

#define HZ (100L)
#define pi (3.14159265)

void setup()
{
  Serial.begin(115200);
  
  pinMode(ledPin, OUTPUT);
  
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = (16L*1000L*1000L)/265L/HZ;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 265 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

volatile int val = 0;

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  val = gyro.getYAxis();
  rawAngle += val - zeroOff;
  // gyro.getYAxis();
  ++samples;
  
 // digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
  
  //tog = !tog;
  //digitalWrite(ledPin,tog?HIGH:LOW);
}

void loop()
{ // double rate = ((rawRate-zeroOff)*14633.0)/1000.0;
  
 // 36/3 = x
  
  long angle = rawAngle * 14633L / HZ / 1000L;
  
  int xAxisValue = accelerometer.getXAxis() - xZero;  
  int yAxisValue = accelerometer.getYAxis() - yZero; 
/*    double rate = ((rawRate-zeroOff)*14633.0)/1000.0;
  //  angle += rate*((double)dt)/1000000.0;
    angle += ((rate+lastRate)*(double)dt)/2000000.0;
  */
//  rawAngle -= 2000;


  Serial.print(angle);
  Serial.print('\t');

  double len = sqrt(yAxisValue*yAxisValue+xAxisValue*xAxisValue);  
  if(len < 105.0) {
    angle = (long)(atan2(yAxisValue,xAxisValue)*180.0/pi);
  }

  Serial.print(yAxisValue);
  Serial.print('\t');
  Serial.print(xAxisValue);
  Serial.print('\t');
  Serial.print(len);
  Serial.print('\t');
  Serial.println(angle);
  
  const int Kp = -75;
  const int Kd = -55;
  
  
  int thro = Kp*angle+Kd*(val-zeroOff);
  
  right.setDirectedThrottle(thro,25);
  left.setDirectedThrottle(thro,25);
}
