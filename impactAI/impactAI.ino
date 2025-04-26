// libraries
#include <WiFi.h>
#include <HTTPClient.h>
#include <PulseSensorPlayground.h>
#include "secrets.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width (px)
#define SCREEN_HEIGHT 64  // OLED display height (px)

// objects
PulseSensorPlayground pulseSensor;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// variables
const int PULSE_INPUT = 34;
const int THRESHOLD = 550;

// api endpoint
const char* server = "https://abod-llm.vercel.app/sensor";

// network info
const char* ssid = SSID;
const char* password = PASSWORD;

// wifi connection
void beginWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Attempting to connect to ");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(" ~");
    delay(1000);
  }
  Serial.println("\nConnected");
}

void setup() {
  Serial.begin(115200);
  delay(1500);
  beginWiFi();

  analogReadResolution(10);

  // pulsesensor manager
  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.setSerial(Serial);
  pulseSensor.setThreshold(THRESHOLD);

  // start reading the pulsesensor signal
  if (!pulseSensor.begin()) {
    Serial.println("Error in creating the pulsesensor signal");
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
}

void loop() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);

  if (pulseSensor.sawStartOfBeat()) {
    int bpm = pulseSensor.getBeatsPerMinute();
    Serial.print("BPM: "); Serial.println(bpm);
    display.print("BPM: "); display.println(bpm);  
    sendBPMData(bpm);

    display.display();
    delay(2000);
  }
  
  delay(10);
}

void sendBPMData(int bpm) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(server);
    http.addHeader("Content-Type", "application/json");

    String json = "{\"pulse\": " + String(bpm) + "}";
    int httpResponseCode = http.POST(json);

    if (httpResponseCode > 0) {
      Serial.print("POST sent. Response: ");
      Serial.println(httpResponseCode);
      display.print("POST OK.Response:"); display.println(httpResponseCode);
    } else {
      Serial.print("POST failed. Error: ");
      Serial.println(http.errorToString(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("WiFi not connected. Can't send BPM.");
  }
}
