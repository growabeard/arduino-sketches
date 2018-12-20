#include "SoftwareSerial.h"
#include <Adafruit_Sensor.h>

#include <DHT_U.h>
#include <DHT.h>

#include <LiteESP8266Client.h>
#include <LiteSerialLogger.h>

//AT+UART_DEF=9600,8,1,0,0 to set esp baud

#define LOGGER Serial

// Radio!
LiteESP8266 radio;

#define DHTTYPE DHT11

int photocellPin = A5;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider

int val = 0; //value for storing moisture value 
int soilPin = A0;//Declare a variable for the soil moisture sensor 
int soilPower = 6;//Variable for Soil moisture Power

static const int DHT_SENSOR_PIN = 2;
DHT dht( DHT_SENSOR_PIN, DHTTYPE );

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Set your SSID and password here, obviously.
// If you don't have a password, you should be able to leave it blank.
//const char ssid[] PROGMEM = "GM Guest WiFi";
//const char password[] PROGMEM = "";
const char ssid[] PROGMEM = "WhateverYouWant";
const char password[] PROGMEM = "chr3ls3y";


const char host[] PROGMEM = "http://monitree.herokuapp.com";
const int port PROGMEM = 80;

const char http_post_request[] PROGMEM = "POST http://monitree.herokuapp.com/readings HTTP/1.1\r\n"
                                          "Host: monitree.herokuapp.com\r\n"
                                          "Content-Type: application/json\r\n";
const char http_post_content_length[] PROGMEM = "Content-length: ";
const char http_post_crlf[] PROGMEM = "\r\n";
const char con_data[] PROGMEM = "{\"name\":\"tree\",\"date\":\",\"temp\":77.00,\"creator\":\"sensor\",\"moisture\":62.00,\"humidity\":38.00,\"light\":69.00,\"watered\":false}";


float sensorData[4];

void setup() {
  LOGGER.begin(9600);
  delay(1000);
  
  radio.begin(115200);

  resetRadioBaud();

  radio.begin(9600);

  // If the radio is already in station mode, there's no need to do this every
  // time.
  if (radio.set_station_mode()) {
    LOGGER.println(F("Set station mode success."));
  } else {
    LOGGER.println(F("Set station mode failed."));
  } 
  dht.begin();
  pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
}

void loop () {
  LOGGER.println("woke up from my nap..");
  connectWifi();
  connectHost();

  //char data[] PROGMEM = "{\"name\":\"tree\",\"date\":\",\"temp\":77.00,\"creator\":\"sensor\",\"moisture\":62.00,\"humidity\":38.00,\"light\":69.00,\"watered\":false}";
  char data[200];
  
  readData(data);

  LOGGER.println("data");
  LOGGER.print(data);
  LOGGER.print(con_data);
  
  sendData(data);
  LOGGER.println("disconnecting from AP");
  disconnectWifi();
//  LOGGER.println("esp sleeping for 3300000 milliseconds.");
//  sleepWifi(300);
  LOGGER.println("sleeping dween for 3600000 milliseconds.");
  delay(1000);
}
