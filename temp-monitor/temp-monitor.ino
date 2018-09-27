#include <Adafruit_Sensor.h>

#include <DHT_U.h>
#include <DHT.h>

// include the library code:
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
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  dht.begin();
  Serial.begin( 9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor

  delay(500);
}

void loop() {
  Serial.print("Reading");
  lcd.setCursor(0,1);
  lcd.print("L " + String(getPhotocell()) + " S " + String(readSoil()));

  String tempHumid = getTempAndHumidity();

  lcd.setCursor(0,0);
  lcd.print(tempHumid);
  delay(10000);
}

int getPhotocell() {
  photocellReading = analogRead(photocellPin);

  return map(photocellReading, 0, 1023, 0, 100);
}

String getTempAndHumidity() {
  float temperature;
  float humidity;
  String temp = "no ";

  temperature = dht.convertCtoF(dht.readTemperature());
  humidity = dht.readHumidity();
  
  temp = "T " + String(temperature) + "* H " + String(humidity) + "%";
  
  return temp;
}

int readSoil()
{

    digitalWrite(soilPower, HIGH);//turn D7 "On"
    delay(10);//wait 10 milliseconds 
    val = analogRead(soilPin);//Read the SIG value form sensor 
    digitalWrite(soilPower, LOW);//turn D7 "Off"
    return val;//send current moisture value
}
