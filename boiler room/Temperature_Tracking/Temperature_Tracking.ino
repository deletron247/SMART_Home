#include "OneWire.h"
#include "DallasTemperature.h"

// Data wire is connected to GPIO15
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

DeviceAddress sensor1 = { 0x28, 0xFF, 0x13, 0xFA, 0x0, 0x17, 0x5, 0xAD };
//DeviceAddress sensor2 = { 0x28, 0xFF, 0xB4, 0x6, 0x33, 0x17, 0x3, 0x4B };
//DeviceAddress sensor3= { 0x28, 0xFF, 0xA0, 0x11, 0x33, 0x17, 0x3, 0x96 };



void setup(void){
  Serial.begin(9600);
  sensors.begin();
  
}

void loop(void){ 
  sensors.requestTemperatures(); // Send the command to get temperatures
  int T1 = sensors.getTempC(sensor1);
  Serial.print("Sensor 1(*C): ");
  Serial.println(T1); 

//  Serial.print("Sensor 2(*C): ");
//  Serial.print(sensors.getTempC(sensor2)); 

//  Serial.print("Sensor 3(*C): ");
//  Serial.print(sensors.getTempC(sensor3)); 

  delay(2000);
}
