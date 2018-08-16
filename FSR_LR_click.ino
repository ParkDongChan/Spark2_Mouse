//1키로 옴 저항(갈검검) - 기준값 180
#include <Mouse.h>


void setup() {
  Serial.begin(9600);
  Mouse.begin();
}
void loop() {
  int sensor_L = analogRead(A0);
  int sensor_R = analogRead(A1);
  if(sensor_L>180){
  Mouse.click(); 
  }
  if(sensor_R>180){
    Mouse.click(MOUSE_RIGHT);
  }
  delay(100);
 Serial .println("Left Sensor : " + String(sensor_L) + "  Right Sensor : " +String(sensor_R) );
  delay(100);
}
