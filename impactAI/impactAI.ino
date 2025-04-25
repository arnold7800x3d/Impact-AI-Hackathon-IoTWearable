// libraries
#include <WiFi.h>
#include <PulseSensorPlayground.h>
#include "secrets.h"

// objects
PulseSensorPlayground pulseSensor;

// variables
const int PULSE_INPUT = 34;
const int THRESHOLD = 685;

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
  int signal = analogRead(PULSE_INPUT);

  if (pulseSensor.sawStartOfBeat()) {
    int bpm = pulseSensor.getBeatsPerMinute();
    Serial.print("BPM: ");
    Serial.println(bpm);
  }

  delay(1000);
}