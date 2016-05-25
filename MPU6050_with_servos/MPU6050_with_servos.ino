
/**
 * @author Aurelio Buarque (abuarquemf@gmail.com)
 * @date 5/24/2016 8:59 PM
 *
 * Seattle, WA
*/

//Used libraries
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Servo.h> 

//Three objects servos
Servo servo1;
Servo servo2;
Servo servo3;

//Define the servo's pins
const byte servo1Pin = 11,
           servo2Pin = 10,
           servo3Pin = 9;

//Object MPU6050
MPU6050 accelgyro;

//These variables stores the values reed by accelerometer and gyroscope (X,Y,Z)
int16_t ax, ay, az;
int16_t gx, gy, gz;

//Usado para calcular o angulo - Variaveis do acelerometro
double accXangle;
double accYangle;
double accZangle;

//Usado para calcular o angulo - Variaveis do giroscopio
double gyroXangle = 180;
double gyroYangle = 180;
double gyroZangle = 180;

//This variable stores the execution time
uint32_t timer;

void setup() {
 
  Wire.begin();

  // Starting comunication serial with baud rate of 384000 bps
  Serial.begin(38400);

  // Starting device
  Serial.println("Starting I2C comunication...");
  accelgyro.initialize();

  // Testing conection with MPU6050
  Serial.println("Testing conection with MPU6050...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 successfully conected" : "Failed to connect to MPU6050");
  
  //Setting servo's pins
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
    
  timer = micros();
}

void loop() {
  
  // Reading MPU6050
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Computing angles with accelerometer
  accXangle = (atan2(ax,az) + PI) * RAD_TO_DEG;
  accYangle = (atan2(ay,az) + PI) * RAD_TO_DEG;
  accZangle = (atan2(ax,ay) + PI) * RAD_TO_DEG;  

  double gyroXrate = ((double)gx / 131.0);
  double gyroYrate = -((double)gy / 131.0);
  double gyroZrate = -((double)gz / 131.0);
  
  //Calculating the angle of rotation without any filter 
  gyroXangle += gyroXrate * ((double)(micros() - timer)/1000000); 
  gyroYangle += gyroYrate * ((double)(micros() - timer)/1000000);
  gyroZangle += gyroZrate * ((double)(micros() - timer)/1000000);
 
  //Make servos write the angles of rotation 
  servo1.write(gyroYangle);
  servo2.write(gyroXangle);
  servo3.write(gyroZangle);

  timer = micros();
  
  //The maximum rate of the accelerometer samples is 1kHz
  delay(1); 

  //Table with values of Accelerometer and gyroscope
  /*
  Serial.print("a/g:\t");
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz); */
}

