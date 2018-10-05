//import Adafruit Unified Sensor & DHT sensor library

#include "SoftwareSerial.h"
#include <Adafruit_Sensor.h>

#include <DHT_U.h>
#include <DHT.h>
#include <LiquidCrystal.h>

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

//String ssid = "monitree-hub";
String ssid = "chrisphone";

//String password = "bungusamongus";
String password = "buttsbutts";

SoftwareSerial esp(4, 5);// RX, TX

String server = "monitree.herokuapp.com"; // www.example.com

String uri = "/monitree/v1/readings";// our example is /esppost.php

float sensorData[4];

void setup() {
  Serial.begin( 9600);
  Serial.println("Starting");
 
  dht.begin();
  pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor

  esp.begin(115200);
  reset();
  connectWifi();
}

//reset the esp8266 module

void reset() {
    Serial.println("reset wifi");
  boolean reset = sendCommand("AT+RST", 5, "OK");

  if (reset) Serial.println("Module Reset");

}

//connect to your wifi network

void connectWifi() {
  Serial.println("connect wifi");
  boolean ssidConnect = sendCommand("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"",5,"OK");

  if (ssidConnect) {
    Serial.println("Connected!");
  } else {
    Serial.println("Cannot connect to wifi");
    connectWifi();
  }
}

String readData () {
  boolean wateredPlant = false;
  Serial.println("reading data");
  String sendPacket = "{\"name\": \"tree\",\"date\": \"\",\"temp\":";

  getTempAndHumidity();

  getSoilMoisture();
  if (sensorData[2] < 25.0) waterPlant(&wateredPlant);
  sendPacket = sendPacket + sensorData[0] + ",\"creator\": \"sensor\",\"moisture\":" + sensorData[2] +",\"humidity\":" + sensorData[1] +",\"watered\":" + String(wateredPlant) + "}";
  Serial.println(sendPacket);
  return sendPacket;
}

void waterPlant(boolean wateredPlant) {
  wateredPlant = false;
}

void loop () {
  
  // convert the bit data to string form

  String data = readData();

  httpPost(data);

  delay(600000);

}

void httpPost (String data) {

  boolean init = sendCommand("AT+CIPSTART=\"TCP\",\"" + server + "\",80", 15, "OK");//start a TCP connection.

  if ( init) {

    Serial.println("TCP connection ready");

  } 

  String postRequest =

    "POST " + uri + " HTTP/1.0\r\n" +

    "Host: " + server + "\r\n" +

    "Accept: *" + "/" + "*\r\n" +

    "Content-Length: " + data.length() + "\r\n" +

    "Content-Type: application/json\r\n" +

    "\r\n" + data;

  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.

  boolean openConn = sendCommand(sendCmd + postRequest.length(), 5, ">");

  if (openConn) {
    Serial.println("Sending.."); 
    
    boolean sendPacket = sendCommand(postRequest, 5, "SEND OK");

    if (sendPacket) {
      Serial.println("Packet sent");

      while (esp.available()) {

        String tmpResp = esp.readString();

        Serial.println(tmpResp);

      }

      // close the connection

      sendCommand("AT+CIPCLOSE", 5, "OK");

    }

  } else {
    sendCommand("AT+CIPCLOSE", 5, "OK");
    httpPost(data);
  }
}

boolean sendCommand(String command, int maxTime, char readReplay[]) {
  int retries = 0;
  boolean found = false;
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while (retries < (maxTime * 1)) {
    esp.println(command);
    if (esp.find(readReplay)) {
      found = true;
      break;
    }
    retries++;
  }
  if (found == true) {
    Serial.println("OYI");
    return found;
  }
  if (found == false) {
    Serial.println("Fail");
    return found;
  }
}

int getPhotocell() {
  photocellReading = analogRead(photocellPin);

  sensorData[3] = map(photocellReading, 0, 1023, 0, 100);
}

void getTempAndHumidity() {
  float temperature;
  float humidity;

  temperature = dht.convertCtoF(dht.readTemperature());
  humidity = dht.readHumidity();
  
  sensorData[0] = temperature;
  sensorData[1] = humidity;
}

void getSoilMoisture()
{
    digitalWrite(soilPower, HIGH);//turn D7 "On"
    delay(10);//wait 10 milliseconds 
    sensorData[2] = map(analogRead(soilPin), 0, 630, 0, 100);//Read the SIG value form sensor 
    digitalWrite(soilPower, LOW);//turn D7 "Off"
}
