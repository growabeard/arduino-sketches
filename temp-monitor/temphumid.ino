void getTempAndHumidity() {
  float temperature;
  float humidity;
  temperature = dht.convertCtoF(dht.readTemperature());
  humidity = dht.readHumidity();
  sensorData[0] = temperature;
  sensorData[1] = humidity;
}
