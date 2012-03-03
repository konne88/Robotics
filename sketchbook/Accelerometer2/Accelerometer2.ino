/*
 Accelerometer

 Reads two analog input pins; a T000020 Accelerometer Module with the x-axis connected to I0 pin
 the y-axis connected to the I1 pin, uses the result to set the brightness of two T010111 LED Module 
 connected on O0 and O1. Also prints the results to the serial monitor.

 created on Dec 2011
 by Federico Vanzati

 This example code is in the public domain.

 */

// include the TinkerKit library
#include <TinkerKit.h>

TKAccelerometer accelerometer(I4, I5);  // creating the object 'accelerometer' that belongs to the 'TKAccelerometer' class 
                       	      		// and giving the values to the desired input pins
               
void setup() 
{
  // initialize serial communications at 9600 bps
  Serial.begin(115200);
}

const int xZero=488;
const int yZero=485;

#define pi (3.14159265)

void loop()
{
  // read the both joystick axis values:
  int xAxisValue = accelerometer.getXAxis() - xZero;  
  int yAxisValue = accelerometer.getYAxis() - yZero; 

  // print the results to the serial monitor:
  Serial.print("Accelerometer X = " );                       
  Serial.print(xAxisValue);   
  Serial.print("\t Accelerometer Y = " );     
  Serial.print(yAxisValue);   
  Serial.print("\t Angle = " );   
  Serial.print(atan2(xAxisValue,yAxisValue)*180.0/pi);     
  Serial.print("\t Length = " );   
  Serial.println(sqrt(yAxisValue*yAxisValue+xAxisValue*xAxisValue));     



  // wait 10 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(10);    
}

