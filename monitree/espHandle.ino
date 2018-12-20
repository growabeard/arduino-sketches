void connectWifi() {
  if (radio.connect_to_ap(ssid, password)) {
    LOGGER.println(F("Join AP success."));
  } else {
    LOGGER.println(F("Join AP failed. Waiting 5 seconds then retrying.."));
    delay(5000);
    connectWifi();
  }
  return;
}

void disconnectWifi() {
  if (radio.disconnect_from_ap()) {
    LOGGER.println(F("Disconnect AP success."));
  } else {
    LOGGER.println(F("Disconnect AP failed. Waiting 5 seconds then retrying.."));
    delay(5000);
    disconnectWifi();
  }
  return;
}

void sleepWifi(const unsigned long sleepTime) {
  if (radio.deep_sleep_radio(sleepTime)) {
    LOGGER.println(F("Sleep AP success."));
  } else {
    LOGGER.println(F("Sleep AP failed. Waiting 5 seconds then retrying.."));
    delay(5000);
    sleepWifi(sleepTime);
  }
  return;
}

void connectHost() {
   if (radio.connect_progmem(host, port)) {
    LOGGER.println(F("Connect to host success."));
  } else {
    LOGGER.println(F("Connect to host failed. Waiting 5 seconds then retrying.."));
    delay(5000);
    connectHost();
  }
  return;
}

void sendData(char *http_post_content) {
  char *data;
  char content_length[8];
  radio.send_progmem(http_post_request);
  radio.send_progmem(http_post_content_length);
  itoa(strlen(http_post_content), content_length, 10);
  radio.send(content_length);
  radio.send_progmem(http_post_crlf);
  radio.send_progmem(http_post_crlf);
  radio.send(http_post_content);
  radio.send_progmem(http_post_crlf);
  radio.close();

  data = radio.get_http_response(512, 10000);
  
  if (data) {
    LOGGER.print(F("Expected 500 bytes, got: "));
    LOGGER.println(strlen(data));
    LOGGER.print(data);
    free(data);
  } else {
    LOGGER.println(F("Error: Data came back null."));
    return false;
  }
}

void resetRadioBaud() {
  if (radio.set_radio_baud(9600)) {
    LOGGER.println(F("baud set to 9600"));
  } else {
    LOGGER.println(F("baud was not set. probably already at 9600."));
  }
}
