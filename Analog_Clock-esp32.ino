#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <LittleFS.h>
#include <time.h>

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "ClockTFT.h"
#include "defines.h"
#include "clock.h"
#include "esp_sntp.h"

#define FORMAT_LITTLEFS_IF_FAILED false

boolean timeIsSet = false;
time_t lastNtpSet = 0;
time_t currentTime = time(nullptr);  // time_t = seconds since epoch
struct tm *timeinfo;
time_t previousEffectTime = time(nullptr);

char ssid[60];
char wifiPassword[60];

ClockTFT tft(TFT_CS, TFT_DC, TFT_RST);

WebServer server(80);

uint8_t currentFaceNumber;
HandPosition hoursHandPositions[15];
HandPosition minutesHandPositions[15];

// Record the NPT set time
void timeUpdated(struct timeval *tv) {
  timeIsSet = true;
  lastNtpSet = time(nullptr);
  Serial.printf("NTP Updated: %s\n", ctime(&lastNtpSet));
}

void printFreeRam() {
  Serial.printf("Free ram: %d bytes\n", ESP.getFreeHeap());
}

void listAllFilesInDir(File dir, int indentation) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) break;

    // make sure to use indentation
    for (uint8_t i = 0; i < indentation; i++) {
      Serial.print("  ");
    }
    Serial.print(entry.name());

    if (entry.isDirectory()) {
      Serial.println("/");

      // recursive file listing inside new directory
      listAllFilesInDir(entry, indentation + 1);
    } else {
      // print file size
      Serial.printf(" - %d\n", entry.size());
    }
    entry.close();
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;  // wait for serial attach
  delay(2000);
  Serial.println();

  Serial.printf("TFT_DIN: %d\n", TFT_DIN);
  Serial.printf("TFT_CLK: %d\n", TFT_CLK);
  Serial.printf("TFT_CS : %d\n", TFT_CS);
  Serial.printf("TFT_DC : %d\n", TFT_DC);
  Serial.printf("TFT_RST: %d\n", TFT_RST);
  Serial.printf("TFT_BL : %d\n", TFT_BL);

  Serial.printf("Flash size: %d\n", ESP.getFlashChipSize());
  printFreeRam();

  // For the ESP the flash has to be read to a buffer
  EEPROM.begin(512);

  // Setup the LCD
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(GC9A01A_BLACK);
  tft.fillCircle(clock_center_x, clock_center_y, SCREEN_DIAMETER / 10, GC9A01A_BLUE);

  int16_t xPos, yPos;
  uint16_t width, height;
  tft.setTextSize(2);

  if (!LittleFS.begin()) {
    Serial.println("LittleFS problem");
    tft.fillScreen(GC9A01A_BLACK);
    String littleFSError = "LittleFS error";
    tft.getTextBounds(littleFSError, 0, 100, &xPos, &yPos, &width, &height);
    tft.setTextColor(GC9A01A_WHITE);
    tft.setCursor(clock_center_x - width / 2, clock_center_y - 50);
    tft.println(littleFSError);
    delay(1000);
  } else {
    // Show some FS info
    Serial.printf("Total space:      %d bytes\n", LittleFS.totalBytes());
    Serial.printf("Total space used: %d bytes\n", LittleFS.usedBytes());
    listAllFilesInDir(LittleFS.open("/"), 0);
  }

  // Setup the wifi
  EEPROM.get(SSID_ADDR, ssid);
  EEPROM.get(WIFI_PASSWORD_ADDR, wifiPassword);
  Serial.printf("\nConnecting to WIFI '%s'...", String(ssid));
  WiFi.mode(WIFI_STA);
  WiFi.hostname(HOSTNAME);
  WiFi.begin(String(ssid), String(wifiPassword));
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Continuing...");
    tft.fillCircle(clock_center_x, clock_center_y, SCREEN_DIAMETER / 10, GC9A01A_RED);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(HOSTNAME);
  } else {
    Serial.println("Connected to WIFI...");
    tft.fillCircle(clock_center_x, clock_center_y, SCREEN_DIAMETER / 10, GC9A01A_GREEN);
  }
  Serial.print("IP address: ");
  String ipAddress = WiFi.localIP().toString();
  Serial.println(ipAddress);

  check_for_updates();

  tft.getTextBounds(ipAddress, 0, 100, &xPos, &yPos, &width, &height);
  tft.setTextColor(GC9A01A_WHITE);
  tft.setCursor(clock_center_x - width / 2, clock_center_y + 50);
  tft.println(ipAddress);
  delay(1000);

  // implement NTP update of timekeeping (with automatic hourly updates)
  configTime(0, 0, NTP_SERVER);
  setenv("TZ", MY_TZ, 1);
  tzset();

  // callback, when NTP changes the time
  sntp_set_time_sync_notification_cb(timeUpdated);
  sntp_set_sync_interval(600 * 1000);  // update every 10 minutes

  setupOTA();

  // Setup the web server
  server.on("/", handleRoot);
  server.on("/wifi", handleWifi);
  server.begin();

  // Initialize face
  randomClockFace();

  Serial.println();
  Serial.println("Running...");
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();

  updateClock();

  static time_t last_check_time = -1;
  time_t now = time(nullptr);
  if (now != last_check_time && now % (3600 * 24) == 0) {
    // run once a day
    last_check_time = now;
    check_for_updates();
  }
}
