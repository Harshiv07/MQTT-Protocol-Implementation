#include <SoftwareSerial.h>

SoftwareSerial BT(0, 1); //TX, RX respetively
String readvoice;

const int trigPin = 9;
const int echoPin = 10;
long duration;
int distanceCm, distanceInch;



void setup() {
 BT.begin(9600);
 Serial.begin(9600);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

//-----------------------------------------------------------------------// 
void loop() {

  while (BT.available()){  //Check if there is an available byte to read
  delay(10); //Delay added to make thing stable
  char c = BT.read(); //Conduct a serial read
  readvoice += c; //build the string- "forward", "reverse", "left" and "right"
  } 

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm= duration*0.034/2;
  distanceInch = duration*0.0133/2;
  delay(10);

  if(distanceCm<20){
    Serial.println("STOP");
    digitalWrite (3, LOW);
    digitalWrite (4, LOW);
    digitalWrite (5, LOW);
    digitalWrite (6, LOW);
    delay(1000);
  }
  else{
    Serial.println("forward");
    Serial.println("FORWARD");
      digitalWrite (3,HIGH);
      digitalWrite (4,LOW);
      digitalWrite (5,LOW);
      digitalWrite (6,LOW);
      delay(200);
      digitalWrite (3,LOW);
      digitalWrite (4,HIGH);
      digitalWrite (5,LOW);
      digitalWrite (6,LOW);
      delay(200);
    digitalWrite(3, HIGH);
    digitalWrite (4, HIGH);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    delay(100);
  }

  if(readvoice == "forward")
  {
      Serial.println("FORWARD");
      digitalWrite (3,HIGH);
      digitalWrite (4,LOW);
      digitalWrite (5,LOW);
      digitalWrite (6,LOW);
      delay(200);
      digitalWrite (3,LOW);
      digitalWrite (4,HIGH);
      digitalWrite (5,LOW);
      digitalWrite (6,LOW);
      delay(200);
      digitalWrite(3, HIGH);
      digitalWrite (4, HIGH);
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      delay(1600);
    }
  
    else if(readvoice == "back")
    {
      Serial.println("BACK");
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6,HIGH);
      delay(2000);
    }
  
    else if (readvoice == "left")
    { 
      Serial.println("LEFT");
      digitalWrite (3,HIGH);
      digitalWrite (4,LOW);
      digitalWrite (5,LOW);
      digitalWrite (6,LOW);
     delay (1000);
        digitalWrite(3, HIGH);
      digitalWrite (4, HIGH);
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      delay(1000);
    
    }
  
   else if ( readvoice == "right")
   {
     Serial.println("RIGHT");
     digitalWrite (3, LOW);
     digitalWrite (4, HIGH);
     digitalWrite (5, LOW);
     digitalWrite (6, LOW);
     delay (1000);
        digitalWrite(3, HIGH);
      digitalWrite (4, HIGH);
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      delay(1000);
   }
  
   else if (readvoice == "stop")
   {
     Serial.println("STOP");
     digitalWrite (3, LOW);
     digitalWrite (4, LOW);
     digitalWrite (5, LOW);
     digitalWrite (6, LOW);
     delay(2000);
   }
   
} //Reset the variable
