/*
 ADXL355-PMDZ Accelerometer Sensor Display

 Shows the output of a ADXL355 accelerometer.
 Uses the SPI library. For details on the sensor, see:
 https://wiki.analog.com/resources/eval/user-guides/eval-adicup360/hardware/adxl355

 Created 22 June 2018
 by Gabriel Vidal
 Fixed 3 Dec 2020
 by Uhwsin
 */

#include <SPI.h>
#include <math.h>
// Memory register addresses:
const int XDATA3 = 0x08;
const int XDATA2 = 0x09;
const int XDATA1 = 0x0A;
const int YDATA3 = 0x0B;
const int YDATA2 = 0x0C;
const int YDATA1 = 0x0D;
const int ZDATA3 = 0x0E;
const int ZDATA2 = 0x0F;
const int ZDATA1 = 0x10;
const int RANGE = 0x2C;
const int POWER_CTL = 0x2D;
const int FILTER_CTL = 0x28;

// Device values
const int RANGE_2G = 0x01;
const int RANGE_4G = 0x02;
const int RANGE_8G = 0x03;
const int MEASURE_MODE_Acce_Only = 0x06; // Only accelerometer
const int MEASURE_MODE_Acce_And_Temp = 0x00; // accelerometer and temp

// Operations
const int READ_BYTE = 0x01;
const int WRITE_BYTE = 0x00;

//Filter Settings
/* 
 * 
 [6:4] HPF_CORNER −3 dB filter corner for the first-order, high-pass filter relative to the ODR 0x0 R/W
 000 Not applicable, no high-pass filter enabled
 001 24.7 × 10−4 × ODR
 010 6.2084 × 10−4 × ODR
 011 1.5545 × 10−4 × ODR
 100 0.3862 × 10−4 × ODR
 101 0.0954 × 10−4 × ODR
 110 0.0238 × 10−4 × ODR
[3:0] ODR_LPF ODR and low-pass filter corner 0x0 R/W
 0000 4000 Hz and 1000 Hz
 0001 2000 Hz and 500 Hz
 0010 1000 Hz and 250 Hz
 0011 500 Hz and 125 Hz
 0100 250 Hz and 62.5 Hz
 0101 125 Hz and 31.25 Hz
 0110 62.5 Hz and 15.625 Hz
 0111 31.25 Hz and 7.813 Hz
 1000 15.625 Hz and 3.906 Hz
 1001 7.813 Hz and 1.953 Hz
 1010 3.906 Hz and 0.977 Hz 
 */
const int HPF_000 = 0x00;
const int HPF_001 = 0x10;
const int HPF_010 = 0x20;
const int HPF_011 = 0x30;
const int HPF_100 = 0x40;
const int HPF_101 = 0x50;
const int HPF_110 = 0x60;
const int LPF_0000  = 0x00;
const int LPF_0001  = 0x01;
const int LPF_0010  = 0x02;
const int LPF_0011  = 0x03;
const int LPF_0100  = 0x04;
const int LPF_0101  = 0x05;
const int LPF_0110  = 0x06;
const int LPF_0111  = 0x07;
const int LPF_1000  = 0x08;
const int LPF_1001  = 0x09;
const int LPF_1010  = 0x0A;

// Pins used for the connection with the sensor
const int CHIP_SELECT_PIN = 7;

void setup() {
  Serial.begin(115200);
  SPI.begin();
 
  // Initalize the  data ready and chip select pins:
  pinMode(CHIP_SELECT_PIN, OUTPUT);

  //Configure ADXL355:
  writeRegister(RANGE, RANGE_2G); // 2G
  writeRegister(POWER_CTL, MEASURE_MODE_Acce_Only); // Enable measure mode
  writeRegister(FILTER_CTL, HPF_000|LPF_0000); // Enable low pass filter 
  // Give the sensor time to set up:
  delay(100);
}

void loop() {

  long axisMeasures[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  int dataSize = 9;
  static double dx,dy,dz,x,y,z;
  // Read accelerometer data
  readMultipleData(XDATA3, dataSize, axisMeasures);

  // Combine data
   static long test;
   long xdata = (axisMeasures[2] >> 4) + (axisMeasures[1] << 4) + (axisMeasures[0] << 12);
   long ydata = (axisMeasures[5] >> 4) + (axisMeasures[4] << 4) + (axisMeasures[3] << 12);
   long zdata = (axisMeasures[8] >> 4) + (axisMeasures[7] << 4) + (axisMeasures[6] << 12);

  //Format convert from  twos complement
  if (xdata >  0x80000 ){   
    xdata -= 0x100000;
  }
  if (ydata >= 0x80000) {
    ydata -= 0x100000;
  }
  if (zdata >= 0x80000) {
    zdata -= 0x100000;
  }
  


  // Print datas


  Serial.print(xdata);
  Serial.print(",");
  Serial.print(ydata);
  Serial.print(",");
  Serial.print(zdata);
  Serial.println();
  


  // Next data in 100 milliseconds
  delay(100);
}

/* 
 * Write registry in specific device address
 */
void writeRegister(byte thisRegister, byte thisValue) {
  byte dataToSend = (thisRegister << 1) | WRITE_BYTE;
  digitalWrite(CHIP_SELECT_PIN, LOW);
  SPI.transfer(dataToSend);
  SPI.transfer(thisValue);
  digitalWrite(CHIP_SELECT_PIN, HIGH);
}

/* 
 * Read registry in specific device address
 */
unsigned int readRegistry(byte thisRegister) {
  unsigned int result = 0;
  byte dataToSend = (thisRegister << 1) | READ_BYTE;

  digitalWrite(CHIP_SELECT_PIN, LOW);
  SPI.transfer(dataToSend);
  result = SPI.transfer(0x00);
  digitalWrite(CHIP_SELECT_PIN, HIGH);
  return result;
}


// * Read  multiple registries
 void readMultipleData(int address, int dataSize, long *readedData) {
  digitalWrite(CHIP_SELECT_PIN, LOW);
  byte dataToSend = (address << 1) | READ_BYTE;
  SPI.transfer(dataToSend);
 
  for(int i = 0; i < dataSize; i = i + 1) {
    readedData[i] = SPI.transfer(0x00);
  }
  digitalWrite(CHIP_SELECT_PIN, HIGH);
}

 

