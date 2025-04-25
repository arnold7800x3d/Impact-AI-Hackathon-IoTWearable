// libraries
#include <PulseSensorPlayground.h>

// variables
const int heartPin = 13;
const int threshold = 550;
const int arraySize = 5;
int sensorReadings[arraySize];
int readingSum = 0;
int validReadings = 0;

// timing variables
unsigned long lastBeatTime = 0;
unsigned long readingInterval = 2000;
unsigned long lastLoopTime = 0;
unsigned long loopDelay = 3000;

// objescts
PulseSensorPlayground pulseSensor;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  pulseSensor.analogInput(heartPin);
  pulseSensor.setThreshold(threshold);

  if(pulseSensor.begin()) {
    Serial.println("The PulseSensor object has been successfully created!!!");
  } else {
    Serial.println("Error in creating PulseSensor object");
  }

  lastLoopTime = millis();
}

void loop() {
  readingSum = 0;
  validReadings = 0;

  while(validReadings < arraySize) {
    if(pulseSensor.sawStartOfBeat()) {
      int bpm = pulseSensor.getBeatsPerMinute();

      if(bpm > 100) {
        sensorReadings[validReadings] = bpm;
        Serial.println("Heartbeat Recorded!!");
        Serial.print("BPM: ");
        Serial.println(sensorReadings[validReadings]);
        readingSum += sensorReadings[validReadings];
        validReadings++;
      }

      lastBeatTime = millis();
    }

    if(millis() - lastBeatTime >= readingInterval) {
      lastBeatTime = millis();
    }
  }
/*
  if(validReadings > 0) {
    int averageBpm = readingSum / validReadings;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("The average BPM is: ");
    lcd.print(averageBpm);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No valid readings");
  }
*/
  lastLoopTime = millis();
  delay(loopDelay);  
}
