#include "SoftwareSerial.h"

#include <Adafruit_Sensor.h>

#include <DHT_U.h>

#include <DHT.h>

#define DHTTYPE DHT11
#define MaxMoisture 660
#define soilPin 0
#define soilPower 6
#define DHT_SENSOR_PIN 2
#define photocellPin 5

int photocellReading; // the analog reading from the sensor divider

int val = 0; //value for storing moisture value

DHT dht(DHT_SENSOR_PIN, DHTTYPE);

String ssid = "WhateverYouWant";
String password = "chr3ls3y";

String server = "monitree.herokuapp.com";
String uri = "/monitree/v1/readings";

SoftwareSerial esp(4, 5); // RX, TX

float sensorData[4];

void setup()
{

  Serial.begin(9600);

  Serial.println("Starting");

  dht.begin();

  pinMode(soilPower, OUTPUT); //Set D7 as an OUTPUT

  digitalWrite(soilPower, LOW); //Set to LOW so no power is flowing through the sensor

  esp.begin(115200);

  reset();

  connectWifi();
}

void loop()
{

  Serial.println("woke up from my nap..");

  // convert the bit data to string form

  String data = readData();

  httpPost(data);

  Serial.println("sleeping for 3600000 milliseconds.");

  delay(3600000);
}

//reset the esp8266 module

void reset()
{

  Serial.println("reset wifi");

  boolean reset = sendCommand("AT+RST", 5, "OK");

  if (reset)
    Serial.println("Module Reset");
}

//connect to your wifi network

void connectWifi()
{

  Serial.println("connect wifi");

  boolean ssidConnect = sendCommand("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"", 5, "OK");

  if (ssidConnect)
  {

    Serial.println("Connected!");
  }
  else
  {

    Serial.println("Cannot connect to wifi");

    connectWifi();
  }
}

String readData()
{

  boolean wateredPlant = false;

  Serial.println("reading data");

  String sendPacket = "{\"name\": \"tree\",\"date\": \"\",\"temp\":";

  getTempAndHumidity();

  getPhotocell();

  getSoilMoisture();

  maybeWaterPlant(sensorData[2], &wateredPlant);

  sendPacket = sendPacket + sensorData[0] + ",\"creator\": \"sensor\",\"moisture\":" + sensorData[2] + ",\"humidity\":" + sensorData[1] + ",\"light\":" + sensorData[3] + ",\"watered\":" + String(wateredPlant) + "}";

  Serial.println(sendPacket);

  return sendPacket;
}

void maybeWaterPlant(float moisture, boolean wateredPlant)
{

  wateredPlant = false;
}

void httpPost(String data)
{

  boolean init = sendCommand("AT+CIPSTART=\"TCP\",\"" + server + "\",80", 15, "OK"); //start a TCP connection.

  if (init)
  {

    Serial.println("TCP connection ready");
  }

  String postRequest =

      "POST " + uri + " HTTP/1.0\r\n" +

      "Host: " + server + "\r\n" +

      "Accept: *" + "/" + "*\r\n" +

      "Content-Length: " + data.length() + "\r\n" +

      "Content-Type: application/json\r\n" +

      "\r\n" + data;

  String sendCmd = "AT+CIPSEND="; //determine the number of caracters to be sent.

  boolean openConn = sendCommand(sendCmd + postRequest.length(), 5, ">");

  if (openConn)
  {

    Serial.println("Sending..");

    boolean sendPacket = sendCommand(postRequest, 5, "SEND OK");

    if (sendPacket)
    {

      Serial.println("Packet sent");

      while (esp.available())
      {

        String tmpResp = esp.readString();

        Serial.println(tmpResp);
      }

      // close the connection

      sendCommand("AT+CIPCLOSE", 5, "OK");
    }
  }
  else
  {

    sendCommand("AT+CIPCLOSE", 5, "OK");

    httpPost(data);
  }
}

boolean sendCommand(String command, int maxTime, char readReplay[])
{

  int retries = 0;

  boolean found = false;

  Serial.print(". at command => ");

  Serial.print(command);

  Serial.print(" ");

  while (retries < (maxTime * 1))
  {

    esp.println(command);

    if (esp.find(readReplay))
    {

      found = true;

      break;
    }

    retries++;
  }

  if (found == true)
  {

    Serial.println("OYI");

    return found;
  }

  if (found == false)
  {

    Serial.println("Fail");

    return found;
  }
}
