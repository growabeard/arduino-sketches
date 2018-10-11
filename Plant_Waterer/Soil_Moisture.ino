void checkSoil() {
  Serial.println("Checking soil sensor");
  digitalWrite(SoilPower, HIGH); //turn D7 "On"
  delay(10); //wait 10 milliseconds
  moistureReading = map(analogRead(SoilPin), 0, MaxMoisture, 0, 100); //Read the moisture level
  digitalWrite(SoilPower, LOW); //turn D7 "Off"
}