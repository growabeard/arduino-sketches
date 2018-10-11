int getPhotocell()
{
  photocellReading = analogRead(photocellPin);
  sensorData[3] = map(photocellReading, 0, 1023, 0, 100);
}
