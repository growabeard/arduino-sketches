//BaneRat

#define MoistureLoopRate 1000
#define SendDataLoopRate 15000
#define SoilPin 0
#define SoilPower 6
#define MaxMoisture 670

#include "Timer.h"

Timer t;

int moistureReading = 0;

void setup()
{
  // initialize serial:
  Serial.begin(9600); 
  Serial.println("Staring");
  t.every(MoistureLoopRate, takeReading);
  t.every(SendDataLoopRate, sendData);
}

void loop()
{
  t.update();
}

void takeReading() {
  checkSoil();
  Serial.println("soil: " + moistureReading);
}

void sendData()
{
  Serial.println("Sending data");
}