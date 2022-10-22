const int speakerPin = 7;
const int inputPin1 = A5;
const int inputPin2 = A4;

void setup() {
	pinMode(speakerPin, OUTPUT);
	pinMode(inputPin1, INPUT);
	pinMode(inputPin2, INPUT);
}

bool isPlayingJumpSound = false;
bool isPlayingDeathSound = false;

void loop() {
	bool pin1IsActive = digitalRead(inputPin1) == HIGH;
	bool pin2IsActive = digitalRead(inputPin2) == HIGH;

  // Jump sound
  if(pin1IsActive && !pin2IsActive) {
    if(!isPlayingJumpSound) {
      isPlayingJumpSound = true;
      jumpSound();
    }

  } else {
    isPlayingJumpSound = false;
  }

  // Death sound
  if(!pin1IsActive && pin2IsActive) {
    if(!isPlayingDeathSound) {
      isPlayingDeathSound = true;
      deathSound();
    }
  } else {
    isPlayingDeathSound = false;
  }

	// if(pin1IsActive && !pin2IsActive && !isPlayingJumpSound) {
	// 	jumpSound();
	// }

	// if(!pin1IsActive && pin2IsActive && !isPlayingDeathSound) {
	// 	deathSound();
	// }

}


void jumpSound() {
  for(int i = 0; i < 20; i++) {
    digitalWrite(speakerPin, HIGH);
    delay(1);
    digitalWrite(speakerPin, LOW);
    delay(1);
  }
}

void deathSound() {
  for(int i = 0; i < 3; i++) {
    digitalWrite(speakerPin, HIGH);
    delay(15);
    digitalWrite(speakerPin, LOW);
    delay(15);
  }

  delay(10);

  for(int i = 0; i < 3; i++) {
    digitalWrite(speakerPin, HIGH);
    delay(15);
    digitalWrite(speakerPin, LOW);
    delay(15);
  }
}