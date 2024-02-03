static void ota_start() {
  Serial.println("OTA update start");
  tft.fillScreen(GC9A01A_BLACK);
  tft.fillCircle(clock_center_x, clock_center_y, 50, GC9A01A_BLUE);
  delay(1000);
  tft.fillScreen(GC9A01A_BLACK);
}

static void ota_on_progress(int progress, int total) {
  static int last_shown = 0;

  int percentage = progress * 100 / total;
  if (last_shown != percentage) {
    if (percentage % 10 == 0) {
      Serial.printf("OTA update progress: %u\r\n", percentage);
    }
    float angle = 2.0 * pi / 110.0 * percentage;
    int x = clock_center_x + (SCREEN_DIAMETER / 2 - 50) * sin(angle);
    int y = clock_center_y - (SCREEN_DIAMETER / 2 - 50) * cos(angle);
    tft.fillCircle(x, y, 10, GC9A01A_YELLOW);

    last_shown = percentage;
  }
}

void setupOTA() {
  // Setup OTA updates
  ArduinoOTA.setPort(3232); // default 3232

  char buffer[50];
  snprintf(buffer, sizeof(buffer), "%s_%d", HOSTNAME, random(9999));
  ArduinoOTA.setHostname(buffer);
  ArduinoOTA.setPassword(PASSWORD); // No authentication by default

  ArduinoOTA.onStart([]() {
    ota_start();
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("OTA update end");
    tft.fillScreen(GC9A01A_BLACK);
    tft.fillCircle(clock_center_x, clock_center_y, 50, GC9A01A_GREEN);
    delay(1000);
    tft.fillScreen(GC9A01A_BLACK);
  });
  ArduinoOTA.onProgress(ota_on_progress);
  ArduinoOTA.onError([](ota_error_t error) {
    char* errorMessage = "Unknown";
    if (error == OTA_AUTH_ERROR) errorMessage = "Auth Failed";
    else if (error == OTA_BEGIN_ERROR) errorMessage = "Begin Failed";
    else if (error == OTA_CONNECT_ERROR) errorMessage = "Connect Failed";
    else if (error == OTA_RECEIVE_ERROR) errorMessage = "Receive Failed";
    else if (error == OTA_END_ERROR) errorMessage = "End Failed";
    Serial.printf("Error[%u]: %s\n", error, errorMessage);
  });
  ArduinoOTA.begin();
}
