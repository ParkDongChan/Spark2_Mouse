//1키로 옴 저항(갈검검) - 기준값 180
#include <Mouse.h>

int RLED = 8;
int GLED = 3;
void setup() {
  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  Serial.begin(9600);
  Mouse.begin();
}
void loop() {
  int sensor_L = analogRead(A0);
  int sensor_R = analogRead(A1);
  if(sensor_L>180){
  digitalWrite(RLED,HIGH);
  Mouse.click(); 
  }
  if(sensor_R>180){
    digitalWrite(GLED,HIGH);
    Mouse.click(MOUSE_RIGHT);
  }
  else{
    digitalWrite(RLED,LOW);
    digitalWrite(GLED,LOW);
  }
  delay(100);
 Serial .println("Left Sensor : " + String(sensor_L) + "  Right Sensor : " +String(sensor_R) );
  delay(100);
}
