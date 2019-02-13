import serial
import requests
import json
import logging
import time

def recvFromArduino():
  global startMarker, endMarker

  x = ser.readline().decode('utf-8')
  if '<' not in x and '>' not in x:
      return(null)
  else:
      return(x.replace('<','').replace('>','').replace('\r\n',''))


def sendToWeb(receivedFromArduino):
    proxies = {
      'http': 'proxy',
      'https': 'proxy',
    }
    URL = "http://monitree.herokuapp.com/readings"
    header = {"Content-Type": "application/json", "Host": "monitree.herokuapp.com", "User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/28.0.1500.52 Safari/537.36"}

    response = requests.post(URL,headers=header,data=receivedFromArduino,proxies=proxies)

    logging.debug("full request: "  + str(response.request.headers) + str(response.request.body) + str(response.request.url))
    logging.debug("response status code: " + str(response.status_code))

logging.basicConfig(level=logging.DEBUG, filename="C:/monitree-logs/log.log", format="%(asctime)s - %(name)s - %(levelname)s - %(message)s")

ser = serial.Serial()
ser.port = 'COM4'
ser.baudrate = 9600
ser.timeout = 1
ser.dtr = False
ser.open()
ser.write(b'\x00')

startMarker = 60
endMarker = 62

logging.debug("Ready")

while True:
  logging.debug("Getting data")
  ser.write("<GET>".encode("utf-8"))
  result = recvFromArduino()
  logging.debug("received from ard: " + result)
  sendToWeb(result)
  logging.debug("Sent to web. Sleeping for 3600..")
  time.sleep(3600)
