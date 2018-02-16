#include <SPI.h>
#include <RF24.h>
#include <BTLE.h>
#include <math.h>
#include "SparkFunLSM6DS3.h"
#include "Wire.h"

int step;
float thrshold, result;

RF24 radio(9,10);

BTLE btle(&radio);
LSM6DS3 myIMU; 
void setup() {

  Serial.begin(9600);
  while (!Serial) { }
  Serial.println("BTLE advertisement sender");
  myIMU.begin();
  btle.begin("group28");
  step = 0;
  thrshold = 13;
  
}

void loop() {
  btle.advertise(0,0);
  btle.hopChannel();
//  Serial.print(".");
  

  result=sqrt(pow(myIMU.readFloatAccelX()*10,2)+pow(myIMU.readFloatAccelY()*10,2)+pow(myIMU.readFloatAccelZ()*10,2));

  
  if (result > thrshold)
  {
    step++;
  }
  Serial.println(step);
  nrf_service_data buf;
  buf.service_uuid = NRF_TEMPERATURE_SERVICE_UUID;
  buf.value = step;

  btle.advertise(0X16, &buf, sizeof(buf));
  btle.hopChannel();
  //  Serial.println('.');
  delay(300);

}

