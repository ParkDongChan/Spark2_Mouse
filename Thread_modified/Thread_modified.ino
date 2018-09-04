#include <Thread.h>
#include <ThreadController.h>
#include <Wire.h>
#include <Mouse.h>

long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;
long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;
int sensor_L = analogRead(A0);
int sensor_R = analogRead(A1);

// ThreadController that will controll all threads
ThreadController controll = ThreadController();


Thread myThread = Thread();
Thread hisThread = Thread();

// callback for myThread
void click(){
  int sensor_L = analogRead(A0); 
  int sensor_R = analogRead(A1);
  Serial .println("Left Sensor : " + String(sensor_L) );
  if(sensor_L>150) {
    Mouse.press(); 
  }
  if(sensor_R>150) {
    Mouse.click(MOUSE_RIGHT);
  }
  else if (sensor_L<150) {
    Mouse.release();
  }
}

// callback for hisThread
void cursor(){
  recordAccelRegisters();
  recordGyroRegisters();
  printData();
}

void setup(){
  // Configure myThread
  myThread.onRun(click);
  myThread.setInterval(200);

  // Configure myThread
  hisThread.onRun(cursor);
  hisThread.setInterval(10);

  // Adds both threads to the controller
  controll.add(&myThread);
  controll.add(&hisThread); // & to pass the pointer to it
  
  Serial.begin(9600);
  Wire.begin();
  setupMPU();
  Mouse.begin();
}

void loop(){
  // run ThreadController
  // this will check every thread inside ThreadController,
  // if it should run. If yes, he will run it;
  controll.run();

  // Rest of code
  float h = 3.1415;
  h/=2;
}

void setupMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}

void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processAccelData();
}

void processAccelData(){
  gForceX = accelX / 20000.0;
  gForceY = accelY / 20000.0; 
  gForceZ = accelZ / 20000.0;
}

void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processGyroData();
}

void processGyroData() {
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0;
  rotZ = gyroZ / 131.0;
}

void printData() {
  Serial.print("Gyro (deg)");
  Serial.print(" X=");
  Serial.print(rotX);
  Serial.print(" Y=");
  Serial.print(rotY);
  Serial.print(" Z=");
  Serial.print(rotZ);
  Serial.print(" Accel (g)");
  Serial.print(" X=");
  Serial.print(gForceX);
  Serial.print(" Y=");
  Serial.print(gForceY);
  Serial.print(" Z=");
  Serial.print(gForceZ);
  Serial.println("L: " + String(sensor_L) + " R: " + String(sensor_R));
  
  if((gForceX>0.15 || gForceX < -0.15) || (gForceY>0.15 || gForceY < -0.15)){
    gForceX*=-20;
    gForceY*=20;
    Mouse.move(gForceX,gForceY);
  }
}
  
