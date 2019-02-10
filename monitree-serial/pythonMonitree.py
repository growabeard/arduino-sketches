import serial
import time
import requests
import json

URL = "http://monitree.herokuapp.com"

header = {"Content-Type": "application/json"}

ser = serial.Serial('COM4', 9600, timeout=0)
var = raw_input("<GET>")
ser.write(var)
while 1:
    try:
      jsonPayload = json.dump(ser.readLine())
      response = requests.post(URL,headers=header,data=jsonPayload)
      print(jsonPayload)
      print(response.status_code)
      time.sleep(1)
    except ser.SerialTimeoutException:
        print('Data could not be read')
