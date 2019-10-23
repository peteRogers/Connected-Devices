
// Include the ESP32 Arduino Servo Library instead of the original Arduino Servo Library
#include <ESP32Servo.h> 

Servo myservo; 
int servoPin = 12;      
int potPin = 34;       
int ADC_Max = 4096;    
                       
  
int val;    
void setup(){ 
  Serial.begin(115200);
  myservo.setPeriodHertz(50);
  myservo.attach(servoPin, 500, 2400);   
}

void loop() {
  val = analogRead(potPin);          
  val = map(val, 0, ADC_Max, 10, 155); 
  myservo.write(val);
  Serial.println(val);
  delay(20);                          
}
