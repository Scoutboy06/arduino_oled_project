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

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "image.h"
#include "image2.h"
#include "white.h"
#include "black.h"

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

int FPS = 2;
bool isFirstImage = true;

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup() {
  Serial.begin(9600);

  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(2000); // wait for initializing
}

void loop() {
  oled.clearDisplay();

  for(int i = 0; i < SCREEN_WIDTH; ++i) {
    for(int j = 0; j < SCREEN_HEIGHT; ++j) {
      int index = j * SCREEN_WIDTH + i;
      oled.setCursor(i, j);

      //? https://learn.adafruit.com/circuitpython-oled-knob-sketcher/drawing-pixels

      //oledImage[index];

      //int8_t i;

      if(isFirstImage) {
        oled.drawPixel(i, j, WHITE);
      } else {
        oled.drawPixel(i, j, BLACK);
      }
    }
  }

  isFirstImage = !isFirstImage;

  oled.display();

  delay(1000/FPS);
}
