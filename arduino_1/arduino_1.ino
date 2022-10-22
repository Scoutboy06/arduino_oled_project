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
#include <string.h>

// #include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "images/dino.h"
#include "images/cactus.h"

#define WIDTH 128 // OLED display width,  in pixels
#define HEIGHT 64 // OLED display height, in pixels

#define BTN_PIN 7
#define ARDUINO_1_PIN_1 2
#define ARDUINO_1_PIN_2 3

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(WIDTH, HEIGHT, &Wire, -1);

void setup() {
  // Serial.begin(9600);
  // pinMode(speakerPin, OUTPUT);
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(2000); // wait for initializing

  oled.clearDisplay();

  pinMode(ARDUINO_1_PIN_1, OUTPUT);
  pinMode(ARDUINO_1_PIN_2, OUTPUT);

  // digitalWrite(ARDUINO_1_PIN, HIGH);
}


int btnState = 0;

const float scrollSpeed = 3.0;
const float G = 0.6;
const float jumpForce = 7.0;
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

bool isPlayingJumpSound = false;
bool isPlayingDeathSound = false;

void loop() {
  digitalWrite(ARDUINO_1_PIN_1, LOW);
  digitalWrite(ARDUINO_1_PIN_2, LOW);

  oled.clearDisplay();
  // oled.fillRect(dinoX, dinoY, dinoWidth, dinoHeight, BLACK);
  // oled.fillRect(cactusX, HEIGHT - cactusHeight, cactusWidth, cactusHeight, BLACK);

  btnState = digitalRead(BTN_PIN);

  // Jump
  if(playerIsOnGround && btnState == HIGH) {
    playerAcc -= jumpForce;
    playerIsOnGround = false;
    digitalWrite(ARDUINO_1_PIN_1, HIGH);
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
    digitalWrite(ARDUINO_1_PIN_2, HIGH);
    reset();
    oled.fillRect(0, 0, WIDTH, HEIGHT, WHITE);
    oled.display();
    delay(1000);
    oled.clearDisplay();
    return;
  }

  drawTexture(dino_image, sizeof(dino_image) / sizeof(dino_image[0]), dinoX, dinoY, dinoWidth, WHITE);
  drawTexture(cactus_image, sizeof(cactus_image) / sizeof(cactus_image[0]), cactusX, HEIGHT - cactusHeight, cactusWidth, WHITE);

  playerAcc = 0;
  oled.display();
}


void drawTexture(unsigned char *texture_image, int size, int offsetX, int offsetY, int width, int color) {
  bool isWhite = !(color == WHITE);
  unsigned int currentIndex = 0;
  unsigned int x;
  unsigned int y;

  for(int i = 0; i < size; i++) {
    if(isWhite) {
      for(int j = 0; j < texture_image[i]; j++) {
        x = currentIndex % width;
        y = floor(currentIndex / width);

        oled.drawPixel(offsetX + x, offsetY + y, WHITE);
        currentIndex++;
      }
    } else {
      currentIndex += texture_image[i];
    }

    isWhite = !isWhite;
  }
}

void reset() {
  cactusX = 140;
  points = 0;
  dinoY = HEIGHT - dinoHeight;
  playerVel = 0;
  playerAcc = 0;
}

void drawPoints() {
  int p = points;

  while(p != 0) {
    int currentNumber = p % 10;

    switch(currentNumber) {
      case 0:
        break;
      case 1:
        break;
      case 2:
        break;
      case 3:
        break;
      case 4:
        break;
      case 5:
        break;
      case 6:
        break;
      case 7:
        break;
      case 8:
        break;
      case 9:
        break;
    }

    p = floor(p / 10);
  }
}
