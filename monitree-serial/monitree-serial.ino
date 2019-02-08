#include "SoftwareSerial.h"
#include <Adafruit_Sensor.h>

#include <DHT_U.h>
#include <DHT.h>

#include <LiteSerialLogger.h>

#define LOGGER Serial

#define DHTTYPE DHT11

int photocellPin = A5;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider

int val = 0; //value for storing moisture value 
int soilPin = A0;//Declare a variable for the soil moisture sensor 
int soilPower = 6;//Variable for Soil moisture Power
static const int PUMP_RELAY_PIN = 7;

static const int DHT_SENSOR_PIN = 2;
DHT dht( DHT_SENSOR_PIN, DHTTYPE );


const int timeout PROGMEM = 3600000;

const int moisture_threshold PROGMEM = 35;

const char con_data[] PROGMEM = "{\"name\":\"tree\",\"date\":\",\"temp\":77.00,\"creator\":\"sensor\",\"moisture\":62.00,\"humidity\":38.00,\"light\":69.00,\"watered\":false}";

float sensorData[4];

void setup() {
  LOGGER.begin(9600);
  delay(1000);
  
  dht.begin();
  pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  digitalWrite(PUMP_RELAY_PIN, LOW);//initialize pump to off?

  // tell the PC we are ready
  LOGGER.println("<Arduino is ready>");
}

void loop () {
  getDataFromPC();
  maybeGetData();
}
