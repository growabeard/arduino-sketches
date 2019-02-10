import serial
import time
import requests
import json



URL = "http://monitree.herokuapp.com"

header = {"Content-Type": "application/json"}

ser = serial.Serial('COM3', 9600, timeout=0)

startMarker = 60
endMarker = 62


def get_job_from_serial():
    response = b'' #buffer for response
    while True:
        try:
            response += ser.read() #read any available data or wait for timeout
            #this technically could only be reading 1 char at a time, but any 
            #remotely modern pc should easily keep up with 9600 baud
        except ser.SerialTimeoutException: #timeout probably means end of data
            #you could also presumably check the length of the buffer if it's always 
            #a fixed length to determine if the entire message has been sent yet.
            break
    return response

def recvFromArduino():
  global startMarker, endMarker
  
  ck = " "
  x = "z" # any value that is not an end- or startMarker
  byteCount = -1 # to allow for the fact that the last increment will be one too many
  
  # wait for the start character
  while len(x) > 0 & ord(x) != startMarker: 
    x = ser.read().decode()
  
  # save data until the end marker is found
  while len(x) > 0 & ord(x) != endMarker:
    if ord(x) != startMarker:
      ck = ck + x 
      byteCount += 1
    x = ser.read().decode()
  
  return(ck)


def waitForArduino():

   # wait until the Arduino sends 'Arduino Ready' - allows time for Arduino reset
   # it also ensures that any bytes left over from a previous message are discarded
   
    global startMarker, endMarker
    
    msg = ""
    while msg.find("Arduino is ready") == -1:

      while ser.inWaiting() == 0:
        pass
        
      msg = recvFromArduino()

      print(msg)




waitForArduino()

ser.write("<GET>".encode())

while 1:
    try:
      result = recvFromArduino()
      print(result)
      jsonPayload = json.dump(ser.read())
      response = requests.post(URL,headers=header,data=jsonPayload)
      print(jsonPayload)
      print(response.status_code)
      time.sleep(1)
    except ser.SerialTimeoutException:
        print('Data could not be read')

