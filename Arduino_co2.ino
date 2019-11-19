// The load resistance on the board
//#define RLOAD 22.0
// Calibration resistance at atmospheric CO2 level
//#define RZERO 879.13

//#define RZERO 76.63
//
//#include "MQ135.h"
//
//const int ANALOGPIN = 2;
//MQ135 gasSensor = MQ135(ANALOGPIN);
//
//
//void setup() {
//  Serial.begin(9600);
//}
// 
//void loop() {
//
//  float zero = gasSensor.getRZero();
//  Serial.print ("rzero: ");
//  Serial.println (zero);
//  float ppm = gasSensor.getPPM();
//  Serial.print ("ppm: ");
//  Serial.println (ppm);
//  delay(1000);
//} 

#include "MQ135.h"

void setup (){
Serial.begin (115200);
}
void loop() {
MQ135 gasSensor = MQ135(A2); // Attach sensor to pin A0
String rzero = (String)gasSensor.getRZero();
if((Serial.available())>0){
  Serial.println (rzero);  
}


delay(5000);
}
