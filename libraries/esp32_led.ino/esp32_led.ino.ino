/*
  ESP32 LED Blink Example
*/

#include “pitches.h”

#define LED 2
#define BUZZER 3
#define BUZZER_FREQUENCY 1000

void setup() {
  // Set pin mode
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}
void loop() {
  delay(1000); // 500ms
  digitalWrite(LED, HIGH); // Turn on LED
  tone(BUZZER, BUZZER_FREQUENCY);
  
  delay(1000); // 500ms
  digitalWrite(LED, LOW); // Turn off LED
  noTone(BUZZER)
}
