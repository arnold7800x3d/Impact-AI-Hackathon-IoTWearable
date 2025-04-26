// libraries
#include <WiFi.h>
#include <HTTPClient.h>
#include <PulseSensorPlayground.h>
#include "secrets.h"

// objects
PulseSensorPlayground pulseSensor;

// variables
const int PULSE_INPUT = 34;
const int THRESHOLD = 685;

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

// bool sendPulseSignal = true;

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
}

void loop() {
  // int signal = analogRead(PULSE_INPUT);

  if (pulseSensor.sawStartOfBeat()) {
    int bpm = pulseSensor.getBeatsPerMinute();
    Serial.print("BPM: ");
    Serial.println(bpm);
    sendBPMData(bpm);
  }

  delay(1000);
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
    } else {
      Serial.print("POST failed. Error: ");
      Serial.println(http.errorToString(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("WiFi not connected. Can't send BPM.");
  }
}
