/*
.____              .___ ____   ____.__    .___             __________.__                             
|    |    ____   __| _/ \   \ /   /|__| __| _/____  ____   \______   \  | _____  ___.__. ___________ 
|    |  _/ ___\ / __ |   \   Y   / |  |/ __ |/ __ \/  _ \   |     ___/  | \__  \<   |  |/ __ \_  __ \
|    |__\  \___/ /_/ |    \     /  |  / /_/ \  ___(  <_> )  |    |   |  |__/ __ \\___  \  ___/|  | \/
|_______ \___  >____ |     \___/   |__\____ |\___  >____/   |____|   |____(____  / ____|\___  >__|   
        \/   \/     \/                     \/    \/                            \/\/         \/    

  Lcd Video Player is an open source video player using arduino and oled screen. 

  Resources:
  https://arduinogetstarted.com/tutorials/arduino-oled
  https://randomnerdtutorials.com/guide-for-oled-display-with-arduino/
  https://www.kjell.com/se/kunskap/hur-funkar-det/arduino/arduino-projekt/visa-text-och-grafik-pa-displayer
	https://javl.github.io/image2cpp/
 */

/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-oled
 */

// :)


#include <math.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "images/dino.h"
#include "images/cactus.h"

#define WIDTH 128 // OLED display width,  in pixels
#define HEIGHT 64 // OLED display height, in pixels
#define SAMPLE_RATE 8000

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(WIDTH, HEIGHT, &Wire, -1);

const int btnPin = 2;
const int speakerPin = 11;

void setup() {
  // Serial.begin(9600);
  pinMode(speakerPin, OUTPUT);
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(2000); // wait for initializing
}


int btnState = 0;

const float scrollSpeed = 3;
const float G = 0.6;
const float jumpForce = 7;
const int dinoX = 20;
const int dinoHeight = 21;
const int dinoWidth = 20;
const int cactusHeight = 20;
const int cactusWidth = 13;

float dinoY = HEIGHT - dinoHeight;
float playerVel = 0;
float playerAcc = 0;
bool playerIsOnGround = true;
float cactusX = 140.0;

bool cactusHasPassed = false;
int points = 0;

void loop() {
  oled.clearDisplay();
  btnState = digitalRead(btnPin);

  // Jump
  if(playerIsOnGround && btnState == HIGH) {
    playerAcc -= jumpForce;
    playerIsOnGround = false;
    jumpSound();
  }

  // Apply forces
  playerAcc += G;
  playerVel += playerAcc;
  dinoY += playerVel;

  cactusX -= scrollSpeed;

  if(cactusX < -cactusWidth) {
    cactusX = 140;
    cactusHasPassed = false;
  }
  
  else if(cactusX < dinoX && !cactusHasPassed) {
    cactusHasPassed = true;
    points++;
  }

  // Ground collision
  if(dinoY + dinoHeight >= HEIGHT) {
    dinoY = HEIGHT - dinoHeight;
    playerVel = 0;
    playerIsOnGround = true;
  }

  // Enemy collision detection
  if((cactusX <= dinoX + dinoWidth) && (cactusX + cactusWidth >= dinoX) && (dinoY + dinoHeight >= HEIGHT - cactusHeight)) {
    reset();
    oled.fillRect(0, 0, WIDTH, HEIGHT, WHITE);
    oled.display();
    deathSound();
    delay(1000);
    return;
  }


  drawPlayer();
  drawCactus();
  // oled.fillRect(cactusX, HEIGHT - cactusHeight, cactusWidth, cactusHeight, WHITE);

  playerAcc = 0;
  oled.display();
}



void reset() {
  cactusX = 140;
  points = 0;
  dinoY = HEIGHT - dinoHeight;
  playerVel = 0;
  playerAcc = 0;
}

void jumpSound() {
  return;
  for(int i = 0; i < 20; i++) {
    digitalWrite(speakerPin, HIGH);
    delay(1);
    digitalWrite(speakerPin, LOW);
    delay(1);
  }
}

void deathSound() {
  return;
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

void drawPlayer() {
  bool isWhite = false;
  unsigned int currentIndex = 0;
  unsigned int x;
  unsigned int y;

  for(int i = 0; i < sizeof(dino_image) / sizeof(dino_image[0]); i++) {
    if(isWhite) {
      for(int j = 0; j < dino_image[i]; j++) {
        x = currentIndex % dinoWidth;
        y = floor(currentIndex / dinoWidth);

        oled.drawPixel(dinoX + x, dinoY + y, WHITE);
        currentIndex++;
      }
    } else {
      currentIndex += dino_image[i];
    }

    isWhite = !isWhite;
  }
}

void drawCactus() {
  bool isWhite = false;
  unsigned int currentIndex = 0;
  unsigned int x;
  unsigned int y;

  for(int i = 0; i < sizeof(cactus_image) / sizeof(cactus_image[0]); i++) {
    if(isWhite) {
      for(int j = 0; j < cactus_image[i]; j++) {
        x = currentIndex % cactusWidth;
        y = floor(currentIndex / cactusWidth);

        oled.drawPixel(cactusX + x, HEIGHT - cactusHeight + y, WHITE);
        currentIndex++;
      }
    } else {
      currentIndex += cactus_image[i];
    }

    isWhite = !isWhite;
  }
}

