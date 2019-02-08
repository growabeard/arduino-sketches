void readData(char *sendPacket) {
  char temp[6];
  char moisture[6];
  char humidity[6];
  char light[6];
  boolean wateredPlant = false;
  Serial.println("reading data");

  getTempAndHumidity();

  getPhotocell();

  getSoilMoisture();
  maybeWaterPlant(sensorData[2], &wateredPlant);
  
  dtostrf(sensorData[0], 3, 2, temp);
  dtostrf(sensorData[2], 3, 2, moisture);
  dtostrf(sensorData[1], 3, 2, humidity);
  dtostrf(sensorData[3], 3, 2, light);
  
  sprintf(sendPacket,"{\"name\":\"tree\",\"date\":\"\",\"temp\":%s,\"creator\":\"sensor\",\"moisture\":%s,\"humidity\":%s,\"light\":%s,\"watered\":%s}",temp,moisture,humidity,light,wateredPlant ? "true" : "false");
}

void maybeWaterPlant(float moisture, boolean wateredPlant) {
  if (moisture < moisture_threshold) {
    digitalWrite(PUMP_RELAY_PIN, HIGH); //turn on pump
    delay(100); //run pump for 10 seconds
    digitalWrite(PUMP_RELAY_PIN, LOW); //turn off pump
    wateredPlant = true;
  } else {
    wateredPlant = false; 
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
    float moisture = analogRead(soilPin);
    sensorData[2] = map(moisture, 0, 670, 0, 100);//Read the SIG value form sensor 
    digitalWrite(soilPower, LOW);//turn D7 "Off"
}
