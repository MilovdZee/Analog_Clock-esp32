#define UPDATE_HOST "raw.githubusercontent.com"
#define UPDATE_PORT 443
#define VERSION_FILE_LOCATION "/MilovdZee/Analog_Clock-esp32/main/firmware/version.txt"
#define FIRMWARE_FILE_LOCATION "/MilovdZee/Analog_Clock-esp32/main/firmware/Analog_Clock-esp32.ino-%d.bin"

#define CONNECTION_TIMEOUT 5000

WiFiClientSecure client;

const char *root_ca =
  "-----BEGIN CERTIFICATE-----\n" 
  "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\n" 
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
