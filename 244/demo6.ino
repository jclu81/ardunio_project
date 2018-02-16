#include <SPI.h>
#include <RF24.h>
#include "SparkFunLSM6DS3.h"
#include "Wire.h"
#include <BTLE.h>

RF24 radio(9,10);
LSM6DS3 myIMU;
double ans;
int num, tmp;
BTLE btle(&radio);

void setup() {

  Serial.begin(9600);
  while (!Serial) { }
  Serial.println("BTLE advertisement sender");
  //Call .begin() to configure the IMU
  myIMU.begin();
  btle.begin("group35");
}

void loop() {
   //Get all parameters
  //以下3行是拷貝的
  nrf_service_data buf;
  buf.service_uuid = NRF_TEMPERATURE_SERVICE_UUID;
  buf.value = BTLE::to_nRF_Float(num);

  //算平方根，把X,Y,Z轴的值扩大了10倍
  ans=sqrt(myIMU.readFloatAccelX()*myIMU.readFloatAccelX()*100+myIMU.readFloatAccelY()*100*myIMU.readFloatAccelY()+myIMU.readFloatAccelZ()*100*myIMU.readFloatAccelZ());
  //13是测试出来的估计值，我们测得13的时候准确度还可以
  if(ans>13){
    num++;
    Serial.println(num);
  }
  //广播
  btle.advertise(0x16,&buf,sizeof(buf));
  btle.hopChannel();
  Serial.println('.');
  delay(300);
}

