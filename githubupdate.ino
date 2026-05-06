#define UPDATE_HOST "raw.githubusercontent.com"
#define UPDATE_PORT 443
#define VERSION_FILE_LOCATION "/MilovdZee/Analog_Clock-esp32/main/firmware/version.txt"
#define FIRMWARE_FILE_LOCATION "/MilovdZee/Analog_Clock-esp32/main/firmware/Analog_Clock-esp32.ino-%d.bin"

#define CONNECTION_TIMEOUT 5000

WiFiClientSecure client;

const char *root_ca =
  "-----BEGIN CERTIFICATE-----\n"
  "MIIFRDCCBCygAwIBAgISBY1Sq3AtD/S2K3R6iv48O7jaMA0GCSqGSIb3DQEBCwUA\n"
  "MDMxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQwwCgYDVQQD\n"
  "EwNSMTIwHhcNMjYwNDA2MjMzMjM2WhcNMjYwNzA1MjMzMjM1WjAWMRQwEgYDVQQD\n"
  "DAsqLmdpdGh1Yi5pbzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOdZ\n"
  "q8I8GHk1g+bKQwY6CYeOw/ZxO3Uv6bGkk3KzLPPKWqLbae+aZIbXLtd1OVyhFeFT\n"
  "jOR8+SxEUE5TpcMSUPUMPQJ54+CO9KoovK5rWiPPpckDoihI3t2HpxLheZPv3mv3\n"
  "smG9SEnZlCDsqFEspbTYUsz8IZwkTYUanSh5jetCumoQ3tDJAQ4NlbQIro1xDFxi\n"
  "qkDt6bYHffznWRsY3LbIxnx2K2sQGgg30/eqtT/nL5uXqlJmK6n1nPtTVXGUWsOl\n"
  "XvvHRkZe1/INk20NCN4SqK21CEwcth9ALqrr/qIv0S/jAUeSXTXXKQbOwB12caao\n"
  "aACXw3n9ztyCJz9/om0CAwEAAaOCAm0wggJpMA4GA1UdDwEB/wQEAwIFoDATBgNV\n"
  "HSUEDDAKBggrBgEFBQcDATAMBgNVHRMBAf8EAjAAMB0GA1UdDgQWBBR0mMdbLJil\n"
  "eTnlyukk8i8OUz8l+DAfBgNVHSMEGDAWgBQAtSnyLY5vMeibTK14Pvrc6QzR0jAz\n"
  "BggrBgEFBQcBAQQnMCUwIwYIKwYBBQUHMAKGF2h0dHA6Ly9yMTIuaS5sZW5jci5v\n"
  "cmcvMGsGA1UdEQRkMGKCDCouZ2l0aHViLmNvbYILKi5naXRodWIuaW+CFyouZ2l0\n"
  "aHVidXNlcmNvbnRlbnQuY29tggpnaXRodWIuY29tgglnaXRodWIuaW+CFWdpdGh1\n"
  "YnVzZXJjb250ZW50LmNvbTATBgNVHSAEDDAKMAgGBmeBDAECATAuBgNVHR8EJzAl\n"
  "MCOgIaAfhh1odHRwOi8vcjEyLmMubGVuY3Iub3JnLzkxLmNybDCCAQsGCisGAQQB\n"
  "1nkCBAIEgfwEgfkA9wB2ANdtfRDRp/V3wsfpX9cAv/mCyTNaZeHQswFzF8DIxWl3\n"
  "AAABnWVZpOoAAAQDAEcwRQIgTaNfb0dM+YvXfEat2Yb5SC7aTmQ1Zq897xwifGai\n"
  "gjMCIQDRAvabapoPoK+VJci9nBB5w/vcX0SUmUlF2X2vryJJ2wB9AEavhj07PuWf\n"
  "pXfeqCRdNrDZ7SKiI/Rhd0EilFLulVBfAAABnWVZpXoACAAABQADMwIRBAMARjBE\n"
  "AiBTuPS20tVu/Hiw8VevhNgtCNasdCwV8cbOrgPjv9UJmAIgZgI47wh8JnNZ1J50\n"
  "ZvPhN0aUMu2zoBy0DLuVyel9uogwDQYJKoZIhvcNAQELBQADggEBAEpWRXLB2tDv\n"
  "pARh6N9d+QsJN2LiNLNsWilOE5SQqFGGFOLJp9ru8fZ6P7BYeNmx6r2JZvGuDdxS\n"
  "gbRkqyNNHOIbfHmNY41OvBSP4Mql2fLghETXZgWNSZFGiMCwalTsMVSS6fxhdAm9\n"
  "rXeCShp3xRuPAoL9qcUv5lE7oqHGUgBa/pUBcss9VAeJBCP43IxrZmVsbOsIS/7P\n"
  "Q5U8GmiQsZaPo80BQMlGHgjpGE8s2o9ot85xDSHDhG4Nhm8bCdL/08aa9Mn1YUdd\n"
  "ny+3nXdMoIqQjCreM7uSAO7MbIeHHD5LAlY8VhFxpNBAMtpFc766hifp5jRWp/Ki\n"
  "/W6lwcki1qI=\n"
  "-----END CERTIFICATE-----\n";

String get_header_value(String header) {
  char *space = strchr(header.c_str(), ' ');
  int index_of_space = space ? space - header.c_str() + 1 : -1;
  if (index_of_space != -1) {
    return header.substring(index_of_space);
  }
  return "";
}

boolean check_for_data() {
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println("Client Timeout!");
      return false;
    }
  }
  return true;
}

long read_content_length_from_headers() {
  long content_length = ERROR_VALUE;
  while (client.available()) {
    String line = client.readStringUntil('\n');
    line.trim();

    // end of headers
    if (line.length() == 0) break;

    // check if the HTTP response is 200
    if (line.startsWith("HTTP/1.1")) {
      if (line.indexOf("200") < 0) {
        Serial.printf("Got a non 200 status code from server: '%s'\n", line.c_str());
        return ERROR_VALUE;
      }
    }

    if (line.startsWith("Content-Length: ")) {
      content_length = atol(get_header_value(line).c_str());
      Serial.printf("  - Content length: %d bytes\n", content_length);
    }

    if (line.startsWith("Content-Type: ")) {
      const char *content_type = get_header_value(line).c_str();
      Serial.printf("  - Content type: '%s'\n", content_type);
    }
  }

  return content_length;
}

boolean connect_to_host() {
  if (WiFi.status() != WL_CONNECTED) return false;

  // set certificate handling
  //client.setInsecure();
  client.setCACert(root_ca);

  // connect to the host
  if (!client.connect(UPDATE_HOST, UPDATE_PORT)) {
    Serial.printf("Connection failed: '%s:%d'\n", UPDATE_HOST, UPDATE_PORT);
    return false;
  }
  Serial.printf("Connected to: '%s:%d'\n", UPDATE_HOST, UPDATE_PORT);

  return true;
}

int get_update_version() {
  if (!connect_to_host()) return ERROR_VALUE;

  // request the version file
  Serial.printf("Requesting '%s'\n", VERSION_FILE_LOCATION);
  String get_request = String("GET ") + VERSION_FILE_LOCATION + " HTTP/1.0\r\n" + "Host: " + UPDATE_HOST + "\r\n" + "User-Agent: ESP32\r\n" + "Cache-Control: no-cache\r\n" + "\r\n";
  client.print(get_request);

  int newest_version = ERROR_VALUE;
  if (check_for_data()) {
    long content_length = read_content_length_from_headers();

    // read the first line of the version file
    if (content_length != ERROR_VALUE && client.available()) {
      String line = client.readStringUntil('\n');
      line.trim();
      newest_version = atoi(line.c_str());
    }
  }
  Serial.printf("Newest firmware version: %d\n", newest_version);

  // stop the connection
  client.stop();

  // validate version
  if (newest_version < 1 || newest_version > 100) return ERROR_VALUE;
  return newest_version;
}

void update_firmware(int version) {
  if (!connect_to_host()) return;

  // request the version file
  char buffer[BUFFER_SIZE];
  snprintf(buffer, BUFFER_SIZE, FIRMWARE_FILE_LOCATION, version);
  Serial.printf("Requesting '%s'\n", buffer);
  String get_request = String("GET ") + buffer + " HTTP/1.0\r\n" + "Host: " + UPDATE_HOST + "\r\n" + "User-Agent: ESP32\r\n" + "\r\n";
  client.print(get_request);

  if (check_for_data()) {
    long content_length = read_content_length_from_headers();
    if (content_length != ERROR_VALUE) {
      bool can_begin = Update.begin(content_length);
      if (can_begin) {
        Serial.printf("Begin OTA of %ld bytes...\n", content_length);
        ota_start();
        Update.onProgress(ota_on_progress);

        size_t written = Update.writeStream(client);

        if (written == content_length) {
          Serial.println("Firmware written successfully");
        } else {
          Serial.println("Firmware not completely written!");
        }

        if (Update.end()) {
          if (Update.isFinished()) {
            Serial.println("Finished OTA");
            ESP.restart();
          } else {
            Serial.println("Unfinished OTA!");
          }
        } else {
          Serial.printf("Update failed: %s\n", Update.getError());
        }
      } else {
        Serial.println("OTA has not enough space!");
        client.flush();
      }
    } else {
      Serial.println("OTA has no data available!");
      client.flush();
    }
  }

  // stop the connection
  client.stop();
}

void check_for_updates() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("Checking for firmware updates... (installed v%d)\n", CURRENT_FIRMWARE_VERSION);
    int newest_version = get_update_version();
    if (newest_version > CURRENT_FIRMWARE_VERSION) {
      Serial.println("Updating firmware...");
      update_firmware(newest_version);
    }
  }
}
