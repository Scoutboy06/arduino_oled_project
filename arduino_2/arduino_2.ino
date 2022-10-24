// #include "notes.h"
#include "PCM.h"
#include "computer6.h"

#define INPUT_PIN 12

const int SAMPLE_RATE = 10000;
const int BITS = 2;

bool isPlaying = false;
unsigned long musicStartedAt = 0;
unsigned int songLength = 0;

void setup() {
  Serial.begin(9600);
	pinMode(INPUT_PIN, INPUT);

  songLength = ((sizeof(defaultSample) * (8 / BITS) ) / SAMPLE_RATE) * 1000;
}

void loop() {
  bool shouldPlay = digitalRead(INPUT_PIN) == HIGH;

  // Start playback
  if(!isPlaying && shouldPlay) {
    startPlayback(defaultSample, sizeof(defaultSample), SAMPLE_RATE, BITS);
    musicStartedAt = millis();
    isPlaying = true;
  }

  // Stop playback
  else if(isPlaying && !shouldPlay) {
    stopPlayback();
    isPlaying = false;
  }

  // If music should loop
  else if(isPlaying && shouldPlay && (millis() - musicStartedAt >= songLength)) {
    stopPlayback();
    startPlayback(defaultSample, sizeof(defaultSample), SAMPLE_RATE, BITS);
    musicStartedAt = millis();
  }
}
