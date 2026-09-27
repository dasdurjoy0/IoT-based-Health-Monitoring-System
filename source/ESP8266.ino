#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

const char* ssid = "Murubbi";
const char* password = "123456000";

unsigned long channelID = 3083330;
const char* writeAPIKey = "EPU4NZ9MLHDR4KRS";

WiFiClient client;

float tempVal = 0;
int spo2Val = 0;

String inputString = "";

void setup() {
  Serial.begin(9600);
  delay(2000);

  Serial.println("BOOT: Starting ESP...");

  connectWiFi();
  ThingSpeak.begin(client);
}

void loop() {
  maintainWiFi();
  readSerialData();
}

// ---------------- WiFi ----------------
void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    // Avoid hanging forever if credentials/AP are wrong; retry loop() will
    // call connectWiFi() again via maintainWiFi().
    if (millis() - startAttempt > 20000) {
      Serial.println("\nWiFi connect timed out, will retry.");
      return;
    }
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void maintainWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
}

// ---------------- Serial Read ----------------
// Accumulates characters until '#', then parses the full frame.
// Ignores anything before a '*' so junk/partial bytes on boot don't
// get misread as a frame.
void readSerialData() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '*') {
      inputString = "*"; // start a new frame, discard any partial junk
    } else if (c == '#') {
      if (inputString.startsWith("*")) {
        inputString += "#";
        processData(inputString);
      }
      inputString = "";
    } else {
      inputString += c;
    }

    // Safety: prevent unbounded growth if '#' never arrives
    if (inputString.length() > 64) {
      inputString = "";
    }
  }
}

// ---------------- Parse Data ----------------
// Expected exact format: *TEMP:36.5,SPO2:98#
// Parsed with indexOf so it does NOT depend on label length,
// unlike a fixed-offset substring() approach.
void processData(String data) {
  Serial.print("RAW DATA: ");
  Serial.println(data);

  int tempLabel = data.indexOf("TEMP:");
  int spo2Label = data.indexOf("SPO2:");
  int comma     = data.indexOf(',');

  if (tempLabel == -1 || spo2Label == -1 || comma == -1) {
    Serial.println("PARSE ERROR: malformed frame");
    return;
  }

  int tempStart = tempLabel + 5; // length of "TEMP:"
  int spo2Start = spo2Label + 5; // length of "SPO2:"

  String tStr = data.substring(tempStart, comma);
  String sStr = data.substring(spo2Start, data.indexOf('#'));

  tempVal = tStr.toFloat();
  spo2Val = sStr.toInt();

  Serial.print("TEMP = ");
  Serial.println(tempVal);
  Serial.print("SPO2 = ");
  Serial.println(spo2Val);

  uploadData();
}

// ---------------- Upload ----------------
void uploadData() {
  Serial.println("Uploading to ThingSpeak...");

  ThingSpeak.setField(1, tempVal);  // Field 1 = Temperature
  ThingSpeak.setField(3, spo2Val);  // Field 3 = SpO2

  int httpCode = ThingSpeak.writeFields(channelID, writeAPIKey);
  Serial.print("HTTP RESPONSE: ");
  Serial.println(httpCode);
  Serial.println("----------------------");

  // ThingSpeak free tier enforces a minimum 15s gap between updates.
  // Arduino sends every 5s, so some uploads WILL be rejected (code 0
  // or non-200). That's expected, not a bug in this code.
}
