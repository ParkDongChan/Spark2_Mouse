#include <Wire.h>
#include "Kalman.h"
#include <Mouse.h>
 
int16_t gyroX, gyroZ;
 
int Sensitivity = 600;
int delayi = 3;
 
uint32_t timer;
uint8_t i2cData[14]; // Buffer for I2C data
 
void setup() {
  Serial.begin(9600);
  Wire.begin();
  i2cData[0] = 7; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
  i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
  
  i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ±2g
  while(i2cWrite(0x19,i2cData,4,false)); // Write to all four registers at once
  while(i2cWrite(0x6B,0x01,true)); // PLL with X axis gyroscope reference and disable sleep mode
  while(i2cRead(0x75,i2cData,1));
  if(i2cData[0] != 0x68) { // Read "WHO_AM_I" register
    Serial.print(F("Error reading sensor"));
    while(1);
  }
 
  delay(100); // Wait for sensor to stabilize
 
  /* Set kalman and gyro starting angle */
  while(i2cRead(0x3B,i2cData,6));
  timer = micros();
 
  Mouse.begin();
}
 
void loop() {
  /* Update all the values */
  while(i2cRead(0x3B,i2cData,14));
  gyroX = ((i2cData[8] << 8) | i2cData[9]);
  gyroZ = ((i2cData[12] << 8) | i2cData[13]);
 
  gyroX = gyroX / Sensitivity / 1.1  * -1;
  gyroZ = gyroZ / Sensitivity  * -1;
 
  Serial.print("\t");
  Serial.print(gyroX);
  Serial.print(gyroZ);
 
  Mouse.move(gyroZ, gyroX);
 
  Serial.print("\r\n");
  delay(delayi);
}
