// include the library code:
#include <LiquidCrystal.h>
#include <dht_nonblocking.h>

#define DHT_SENSOR_TYPE DHT_TYPE_11


static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  Serial.begin( 9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
}

void loop() {

  float temperature;
  float humidity;

  if(dht_sensor.measure(&temperature, &humidity)){
    String tempF = String(temperature * 1.8 + 32);
    lcd.setCursor(0,0);
    String temp = "T " + tempF + "* H " + String(humidity) + "%";
    lcd.print(temp);
  }
}