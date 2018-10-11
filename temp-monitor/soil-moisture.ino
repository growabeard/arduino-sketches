void getSoilMoisture() {
  digitalWrite(soilPower, HIGH); //turn D7 "On"
  delay(10); //wait 10 milliseconds
  sensorData[2] = map(analogRead(soilPin), 0, MaxMoisture, 0, 100); //Read the moisture level
  digitalWrite(soilPower, LOW); //turn D7 "Off"
}
