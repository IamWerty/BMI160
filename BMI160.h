#ifndef BMI_CONFIG
#define BMI_CONFIG

#include <Wire.h>

// I2C Address
#define BMI160_ADDR         0x68

// Idenify
#define REG_CHIP_ID         0x00  // повертає 0xD1 (209)

// Commands
#define REG_CMD             0x7E
#define CMD_SOFT_RESET      0xB6
#define CMD_ACC_NORMAL      0x11
#define CMD_GYR_NORMAL      0x15

// Accelerometr config 
#define REG_ACC_CONF        0x40
#define REG_ACC_RANGE       0x41
#define ACC_RANGE_2G        0x03
#define ACC_RANGE_4G        0x05
#define ACC_RANGE_8G        0x08
#define ACC_RANGE_16G       0x0C

// Gyrometr config
#define REG_GYR_CONF        0x42
#define REG_GYR_RANGE       0x43
#define GYR_RANGE_2000      0x00
#define GYR_RANGE_1000      0x01
#define GYR_RANGE_500       0x02
#define GYR_RANGE_250       0x03
#define GYR_RANGE_125       0x04

// Accelerometr Data
#define REG_ACC_X_LOW       0x12
#define REG_ACC_X_HIGH      0x13
#define REG_ACC_Y_LOW       0x14
#define REG_ACC_Y_HIGH      0x15
#define REG_ACC_Z_LOW       0x16
#define REG_ACC_Z_HIGH      0x17

// Gyrometr Data
#define REG_GYR_X_LOW       0x0C
#define REG_GYR_X_HIGH      0x0D
#define REG_GYR_Y_LOW       0x0E
#define REG_GYR_Y_HIGH      0x0F
#define REG_GYR_Z_LOW       0x10
#define REG_GYR_Z_HIGH      0x11

/*
  Difference between LOW and HIGH register on ACC and GYR:
  Low:                      High:
      -Lower energy using        -A LOT MORE energy using
      -Lower accuracy            -Higher accuracy
      -More noises               -Less noises
      -Less update rate          -Higher update rate
*/

// Temperature(dont use for now)
#define REG_TEMP_LOW        0x20
#define REG_TEMP_HIGH       0x21

// Statuses
#define REG_STATUS          0x03
#define REG_ERR             0x02

// Scale (LSB per one)
#define ACC_SCALE_2G        16384.0f // "f" for float, for faster calc
#define ACC_SCALE_4G        8192.0f
#define ACC_SCALE_8G        4096.0f
#define ACC_SCALE_16G       2048.0f
#define GYR_SCALE_2000      16.384f
#define GYR_SCALE_1000      32.768f
#define GYR_SCALE_500       65.536f
#define GYR_SCALE_250       131.072f
#define GYR_SCALE_125       262.144f

// lowlevel funcs

uint8_t readReg(uint8_t reg) {
  Wire.beginTransmission(BMI160_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)BMI160_ADDR, (uint8_t)1, (uint8_t)1);
  return Wire.read();
}

void writeReg(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(BMI160_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission(true);
}

int16_t readReg16(uint8_t regLow) { // need for reading full coordinates in one itteration
  Wire.beginTransmission(BMI160_ADDR);
  Wire.write(regLow);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)BMI160_ADDR, (uint8_t)2, (uint8_t)1);
  uint8_t low  = Wire.read();
  uint8_t high = Wire.read();
  return (int16_t)((high << 8) | low);
}

// Structures

struct SensorData {
  float ax, ay, az;  // g
  float gx, gy, gz;  // °/s
};

struct Offset {
  float ax, ay, az;
  float gx, gy, gz;
};

// Sensor Class

class BMI160 {
  private:
    float accScale  = ACC_SCALE_4G;    // default ±4g
    float gyroScale = GYR_SCALE_500;   // default ±500°/s

    void softReset() {writeReg(REG_CMD, CMD_SOFT_RESET);}
    void accNormalMode() {writeReg(REG_CMD, CMD_ACC_NORMAL);}
    void gyroNormalMode() {writeReg(REG_CMD, CMD_GYR_NORMAL);}

  public:
	bool isAlive() {
		return (readReg(REG_CHIP_ID) == 0xD1);
	}
  
    void connectionTest() {
      uint8_t id = readReg(REG_CHIP_ID);
      Serial.print("CHIP_ID: ");
      Serial.println(id);  // if print "209" (In HEX:"0xD1") - connection fine
	  if (isAlive()) {
		Serial.println("З'єднання OK");
		} else {
			Serial.println("Помилка з'єднання!");
		}
    }

    void setAccRange(int g) {
      switch(g) {
        case 2:
          writeReg(REG_ACC_RANGE, ACC_RANGE_2G);
          accScale = ACC_SCALE_2G;
          break;
        case 4:
          writeReg(REG_ACC_RANGE, ACC_RANGE_4G);
          accScale = ACC_SCALE_4G;
          break;
        case 8:
          writeReg(REG_ACC_RANGE, ACC_RANGE_8G);
          accScale = ACC_SCALE_8G;
          break;
        case 16: 
          writeReg(REG_ACC_RANGE, ACC_RANGE_16G);
          accScale = ACC_SCALE_16G;
          break;
        default:
          Serial.println("Set Accelerometr Range! (2/4/8/16)");
      }
    }

    void setGyroRange(int hz) {
      switch(hz) {
        case 125:
          writeReg(REG_GYR_RANGE, GYR_RANGE_125);  gyroScale = GYR_SCALE_125;
          break;
        case 250:
          writeReg(REG_GYR_RANGE, GYR_RANGE_250);  gyroScale = GYR_SCALE_250;
          break;
        case 500:
          writeReg(REG_GYR_RANGE, GYR_RANGE_500);
          gyroScale = GYR_SCALE_500;
          break;
        case 1000:
          writeReg(REG_GYR_RANGE, GYR_RANGE_1000);
          gyroScale = GYR_SCALE_1000; 
          break;
        case 2000:
          writeReg(REG_GYR_RANGE, GYR_RANGE_2000);
          gyroScale = GYR_SCALE_2000;
          break;
        default: Serial.println("Set Gyro Range! (125/250/500/1000/2000)");
      }
    }

    void init(int accG, int gyroHz) { // Initialization pipeline
      softReset();      
      delay(100);
      accNormalMode();  
      delay(80);
      gyroNormalMode(); 
      delay(80);
      setAccRange(accG);
      setGyroRange(gyroHz);
    }
	
    SensorData read() {
      SensorData d;
      // accScale & gyroScale alredy in init() — divine by them
      d.ax = readReg16(REG_ACC_X_LOW) / accScale;
      d.ay = readReg16(REG_ACC_Y_LOW) / accScale;
      d.az = readReg16(REG_ACC_Z_LOW) / accScale;
      d.gx = readReg16(REG_GYR_X_LOW) / gyroScale;
      d.gy = readReg16(REG_GYR_Y_LOW) / gyroScale;
      d.gz = readReg16(REG_GYR_Z_LOW) / gyroScale;
      return d;
    }

    Offset calibrate(int samples = 500) {
      Offset off = {0};
      for (int i = 0; i < samples; i++) {
        SensorData d = read();
        off.ax += d.ax; 
        off.ay += d.ay; 
        off.az += d.az;

        off.gx += d.gx; 
        off.gy += d.gy; 
        off.gz += d.gz;
        delay(4);
      }
      off.ax /= samples; 
      off.ay /= samples; 
      off.az /= samples;

      off.gx /= samples; 
      off.gy /= samples; 
      off.gz /= samples;
      off.az -= 1.0f; // remove gravity effect
      return off;
    }

    SensorData readCalibrated(const Offset& off) { // use "&" to avoid duplicates and work with the same obj(optimizate memory)
      SensorData d = read();
      d.ax -= off.ax;
      d.ay -= off.ay;
      d.az -= off.az;

      d.gx -= off.gx;
      d.gy -= off.gy;
      d.gz -= off.gz;
      return d;
    }
};

#endif