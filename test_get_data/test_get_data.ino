/************************************************************
ESP8266_Shield_Demo.h
SparkFun ESP8266 AT library - Demo
Jim Lindblom @ SparkFun Electronics
Original Creation Date: July 16, 2015
https://github.com/sparkfun/SparkFun_ESP8266_AT_Arduino_Library

This example demonstrates the basics of the SparkFun ESP8266
AT library. It'll show you how to connect to a WiFi network,
get an IP address, connect over TCP to a server (as a client),
and set up a TCP server of our own.

Development environment specifics:
  IDE: Arduino 1.6.5
  Hardware Platform: Arduino Uno
  ESP8266 WiFi Shield Version: 1.0

This code is released under the MIT license.

Distributed as-is; no warranty is given.
************************************************************/

//////////////////////
// Library Includes //
//////////////////////
// SoftwareSerial is required (even you don't intend on
// using it).
#include <SoftwareSerial.h> 
#include <SparkFunESP8266WiFi.h>

//////////////////////////////
// WiFi Network Definitions //
//////////////////////////////
// Replace these two character strings with the name and
// password of your WiFi network.
const char mySSID[] = "GM Guest WiFi";
const char myPSK[] = "";


//////////////////
// HTTP Strings //
//////////////////
const char destServer[] = "monitree.herokuapp.com";

const String httpRequest = "GET /monitree/v1/readings/13 HTTP/1.1\r\n"
                           "Host: monitree.herokuapp.com\r\n"
                           "Content-Type: application/json\r\n\r\n";
                           
// All functions called from setup() are defined below the
// loop() function. They modularized to make it easier to
// copy/paste into sketches of your own.
void setup() 
{
  // Serial Monitor is used to control the demo and view
  // debug information.
  Serial.begin(9600);

  // initializeESP8266() verifies communication with the WiFi
  // shield, and sets it up.
  initializeESP8266();

  // connectESP8266() connects to the defined WiFi network.
  connectESP8266();

  // displayConnectInfo prints the Shield's local IP
  // and the network it's connected to.
  displayConnectInfo();
}

void loop() 
{
  clientDemo();
  delay(50000);
}

void initializeESP8266()
{
  // esp8266.begin() verifies that the ESP8266 is operational
  // and sets it up for the rest of the sketch.
  // It returns either true or false -- indicating whether
  // communication was successul or not.
  // true
  int test = esp8266.begin(9600);
  if (test != true)
  {
    Serial.println(F("Error talking to ESP8266."));
  }
  Serial.println(F("ESP8266 Shield Present"));
}

void connectESP8266()
{
  // The ESP8266 can be set to one of three modes:
  //  1 - ESP8266_MODE_STA - Station only
  //  2 - ESP8266_MODE_AP - Access point only
  //  3 - ESP8266_MODE_STAAP - Station/AP combo
  // Use esp8266.getMode() to check which mode it's in:
  int retVal = esp8266.getMode();
  if (retVal != ESP8266_MODE_STA)
  { // If it's not in station mode.
    // Use esp8266.setMode([mode]) to set it to a specified
    // mode.
    retVal = esp8266.setMode(ESP8266_MODE_STA);
    if (retVal < 0)
    {
      Serial.print(F("Error setting mode. "));
      Serial.println(retVal);
    }
  }
  Serial.println(F("Mode set to station"));

  // esp8266.status() indicates the ESP8266's WiFi connect
  // status.
  // A return value of 1 indicates the device is already
  // connected. 0 indicates disconnected. (Negative values
  // equate to communication errors.)
  retVal = esp8266.status();
  if (retVal <= 0)
  {
    Serial.print(F("Connecting to "));
    Serial.println(mySSID);
    // esp8266.connect([ssid], [psk]) connects the ESP8266
    // to a network.
    // On success the connect function returns a value >0
    // On fail, the function will either return:
    //  -1: TIMEOUT - The library has a set 30s timeout
    //  -3: FAIL - Couldn't connect to network.
    retVal = esp8266.connect(mySSID, myPSK);
    if (retVal < 0)
    {
      Serial.print(F("Error connecting "));
      Serial.println(retVal);
    }
  }
}

void displayConnectInfo()
{
  char connectedSSID[24];
  memset(connectedSSID, 0, 24);
  // esp8266.getAP() can be used to check which AP the
  // ESP8266 is connected to. It returns an error code.
  // The connected AP is returned by reference as a parameter.
  int retVal = esp8266.getAP(connectedSSID);
  if (retVal > 0)
  {
    Serial.print(F("Connected to: "));
    Serial.println(connectedSSID);
  }

  // esp8266.localIP returns an IPAddress variable with the
  // ESP8266's current local IP address.
  IPAddress myIP = esp8266.localIP();
  Serial.print(F("My IP: ")); Serial.println(myIP);
}

void clientDemo()
{
  // To use the ESP8266 as a TCP client, use the 
  // ESP8266Client class. First, create an object:
  ESP8266Client client;


//  int16_t resp = client.ping("monitree.herokuapp.com");
//  Serial.print("Ping " + String(resp));
  // ESP8266Client connect([server], [port]) is used to 
  // connect to a server (const char * or IPAddress) on
  // a specified port.
  // Returns: 1 on success, 2 on already connected,
  // negative on fail (-1=TIMEOUT, -3=FAIL).
  int retVal = client.connect(destServer, 80);
  if (retVal <= 0)
  {
    Serial.print(F("Failed to connect to server."));
    Serial.println(retVal);
    return;
  }
  Serial.print(httpRequest);
  // print and write can be used to send data to a connected
  // client connection.
  client.print(httpRequest);

  delay(5000);
  // available() will return the number of characters
  // currently in the receive buffer.
  while (client.available())
    Serial.write(client.read()); // read() gets the FIFO char
  
  // connected() is a boolean return value - 1 if the 
  // connection is active, 0 if it's closed.
  if (client.connected())
    client.stop(); // stop() closes a TCP connection.
}
