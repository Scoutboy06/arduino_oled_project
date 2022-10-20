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
// #include "U8glib.h"

// #include <string.h>

// #include "frameVideos/never_gonna_give_you_up.h"
#include "images/dino.h"

#define WIDTH 128 // OLED display width,  in pixels
#define HEIGHT 64 // OLED display height, in pixels
#define FPS 12

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(WIDTH, HEIGHT, &Wire, -1);
// U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE);

const int btnPin = 2;

void setup() {
  Serial.begin(9600);

  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(2000); // wait for initializing
}

int btnState = 0;

const float scrollSpeed = 3;
const float G = 0.6;
const float jumpForce = 6;
const int dinoX = 20;
const int dinoHeight = 21;
const int dinoWidth = 20;
const int cactusHeight = 20;
const int cactusWidth = 10;

float dinoY = HEIGHT - dinoHeight;
float playerVel = 0;
float playerAcc = 0;
bool playerIsOnGround = true;
float cactusX = 100.0;

void loop() {
  oled.clearDisplay();
  btnState = digitalRead(btnPin);

  // Jump
  if(playerIsOnGround && btnState == HIGH) {
    playerAcc -= jumpForce;
    playerIsOnGround = false;
  }

  // Apply forces
  playerAcc += G;
  playerVel += playerAcc;
  dinoY += playerVel;

  // cactusX -= scrollSpeed;

  if(cactusX < -cactusWidth) {
    cactusX = 140;
  }

  // Ground collision
  if(dinoY + dinoHeight >= HEIGHT) {
    dinoY = HEIGHT - dinoHeight;
    playerVel = 0;
    playerIsOnGround = true;
  }

  // Enemy collision detection
  if((cactusX <= dinoX + dinoWidth) && (cactusX + cactusWidth >= dinoX) && (dinoY + dinoHeight >= HEIGHT - cactusHeight)) {
    oled.fillRect(0, 0, WIDTH, HEIGHT, WHITE);
    oled.display();
    delay(1000);
  }


  // oled.fillRect(dinoX, dinoY, dinoWidth, dinoHeight, WHITE);
  drawPlayer();
  oled.fillRect(cactusX, HEIGHT - cactusHeight, cactusWidth, cactusHeight, WHITE);

  playerAcc = 0;
  oled.display();
  // delay(1000 / FPS);

  // delay(500);
}



void drawPlayer() {
  bool isWhite = false;
  unsigned int currentIndex = 0;
  unsigned int x;
  unsigned int y;

  for(int i = 0; i < 62; i++) {
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