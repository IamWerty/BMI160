#include <Wire.h>

#include "BMI160.h"

BMI160 BMI; // This class for behavior of sensor
Offset off; // This structure for data. TODO: buffer

void setup() {
  Serial.begin(115200);
  Wire.begin();
  BMI.init(4, 500);        // +-4g, +-500 degrees per second
  off = BMI.calibrate();
  BMI.connectionTest();
}

void loop() {
  SensorData d = BMI.readCalibrated(off);
  Serial.print("ax:"); Serial.print(d.ax);
  Serial.print(" | ay:"); Serial.print(d.ay);
  Serial.print(" | az:"); Serial.println(d.az);

  Serial.print("gx:"); Serial.print(d.gx);
  Serial.print(" | gy:"); Serial.print(d.gy);
  Serial.print(" | gz:"); Serial.println(d.gz);
}