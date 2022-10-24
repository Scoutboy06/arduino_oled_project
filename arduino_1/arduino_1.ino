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

#include "images/stickman.h"
#include "images/hurdle.h"
#include "images/redo.h"

#define WIDTH 128 // OLED display width,  in pixels
#define HEIGHT 64 // OLED display height, in pixels

#define SPEAKER_PIN 11
#define BTN_PIN 7
#define ARDUINO_1_PIN_1 2
#define ARDUINO_1_PIN_2 3
#define STICKMAN_FRAMES_PER_FRAME 2

#define PLAYER_TEXTURE_HEIGHT 19
#define PLAYER_TEXTURE_WIDTH 21
// Hitbox is aligned to the top-right corner of the texture
#define PLAYER_HITBOX_HEIGHT 12
#define PLAYER_HITBOX_WIDTH 14

#define HURDLE_WIDTH 7
#define HURDLE_HEIGHT 14

#define NUM_ENTRIES(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(WIDTH, HEIGHT, &Wire, -1);

char btnState = 0;

const float G = 0.6;
const unsigned char jumpForce = 5;
const unsigned char playerX = 20;

float scrollSpeed = 3.0;
float playerY = HEIGHT - PLAYER_TEXTURE_HEIGHT;
float playerVel = 0.0;
float playerAcc = 0.0;
bool playerIsOnGround = true;
float hurdleX = 140.0;

bool hurdleHasPassed = false;
unsigned int points = 0;

unsigned long frameCount = 0;

bool isStartScreen = true;
bool isGameOverScreen = false;

void setup() {
  Serial.begin(9600);

  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(ARDUINO_1_PIN_1, OUTPUT);
  pinMode(ARDUINO_1_PIN_2, OUTPUT);

  delay(2000); // wait for initializing
  oled.clearDisplay();
  digitalWrite(ARDUINO_1_PIN_1, HIGH);
}

void loop() {
  oled.clearDisplay();

  btnState = digitalRead(BTN_PIN);

  if(isStartScreen) {
    drawStartScreen();
    return;
  } else if(isGameOverScreen) {
    drawGameOverScreen();
    return;
  }

  // Jump
  if(playerIsOnGround && btnState == HIGH) {
    playerAcc -= jumpForce;
    playerIsOnGround = false;
    tone(SPEAKER_PIN, 500, 40);
  }

  // Apply forces
  playerAcc += G;
  playerVel += playerAcc;
  playerY += playerVel;

  hurdleX -= scrollSpeed;

  // Reset hurdle position
  if(hurdleX <= -HURDLE_WIDTH) {
    hurdleX = WIDTH;
    hurdleHasPassed = false;
  }
  
  // Give point
  else if(hurdleX < playerX && !hurdleHasPassed) {
    hurdleHasPassed = true;
    points++;
  }

  // Ground collision
  if(playerY + PLAYER_TEXTURE_HEIGHT >= HEIGHT) {
    playerY = HEIGHT - PLAYER_TEXTURE_HEIGHT;
    playerVel = 0;
    playerIsOnGround = true;
  }

  // Enemy collision detection
  if(playerCollidesWithHurdle()) {
    digitalWrite(ARDUINO_1_PIN_1, LOW);

    isGameOverScreen = true;

    tone(SPEAKER_PIN, 33, 200);
    // delay(110);
    // tone(SPEAKER_PIN, 33, 100);

    oled.clearDisplay();
    return;
  }

  char stickmanFrame = int(floor(frameCount / STICKMAN_FRAMES_PER_FRAME)) % NUM_ENTRIES(stickman_running);

  if(playerIsOnGround) drawTexture(stickman_running[stickmanFrame], NUM_ENTRIES(stickman_running[stickmanFrame]), playerX, playerY, PLAYER_TEXTURE_WIDTH, WHITE);
  else {
    if(playerVel > 0 && playerY > HEIGHT - PLAYER_TEXTURE_HEIGHT - 6) drawTexture(stickman_static_in_jump, NUM_ENTRIES(stickman_static_in_jump), playerX, playerY, PLAYER_TEXTURE_WIDTH, WHITE);
    else if(playerVel < -1) drawTexture(stickman_jumping, NUM_ENTRIES(stickman_jumping), playerX, playerY, PLAYER_TEXTURE_WIDTH, WHITE);
    else if(playerVel > 1) drawTexture(stickman_falling, NUM_ENTRIES(stickman_falling), playerX, playerY, PLAYER_TEXTURE_WIDTH, WHITE);
    else drawTexture(stickman_static_in_jump, NUM_ENTRIES(stickman_static_in_jump), playerX, playerY, PLAYER_TEXTURE_WIDTH, WHITE);
  }

  drawTexture(hurdle, NUM_ENTRIES(hurdle), hurdleX, HEIGHT - HURDLE_HEIGHT, HURDLE_WIDTH, WHITE);
  drawPoints(5);

  playerAcc = 0;
  frameCount++;
  scrollSpeed += 0.002;

  oled.display();
}


bool playerCollidesWithHurdle() {
  unsigned char hitboxX = playerX + (PLAYER_TEXTURE_WIDTH - PLAYER_HITBOX_WIDTH);
  unsigned char hitboxY = playerY + (PLAYER_TEXTURE_HEIGHT - PLAYER_HITBOX_HEIGHT);

  return (hurdleX <= hitboxX + PLAYER_HITBOX_WIDTH) && (hurdleX + HURDLE_WIDTH >= hitboxX) && (playerY + PLAYER_HITBOX_HEIGHT >= HEIGHT - HURDLE_HEIGHT);
}


void drawTexture(unsigned char *texture_image, int size, int offsetX, int offsetY, int width, int color) {
  unsigned int currentIndex = 0;
  unsigned int x;
  unsigned int y;
  bool isWhite = !(color == WHITE);

  for(int i = 0; i < size; i++) {
    int n = pgm_read_byte(&texture_image[i]);

    if(isWhite) {
      for(int j = 0; j < n; j++) {
        x = currentIndex % width;
        y = floor(currentIndex / width);

        oled.drawPixel(offsetX + x, offsetY + y, WHITE);
        currentIndex++;
      }
    } else {
      currentIndex += n;
    }

    isWhite = !isWhite;
  }
}

void reset() {
  hurdleX = 140;
  points = 0;
  playerY = HEIGHT - PLAYER_TEXTURE_HEIGHT;
  playerVel = 0;
  playerAcc = 0;
  scrollSpeed = 3.0;
}

void drawPoints(int y) {
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(WIDTH - 20, y);

  if(points == 0) {
    oled.print("000");
    return;
  }

  String p = String(points);
  char digits = floor(log10(points)) + 1;
  char diff = 3 - digits;

  for(int i = 0; i < diff; i++) {
    p = "0" + p;
  }

  oled.print(p);
}

void drawStartScreen() {
  oled.setTextSize(2);
  oled.setTextColor(WHITE);

  oled.setCursor(30, 10);
  oled.println("HURDLE");


  if(btnState == HIGH) {
    oled.fillRect(30, 32, WIDTH - 60, 30, WHITE);

    oled.setTextColor(BLACK);
    oled.setTextSize(2);
    oled.setCursor(40, 40);
    oled.println("PLAY");

    oled.display();

    delay(200);
    isStartScreen = false;

    return;
  }

  
  oled.drawRect(30, 32, WIDTH - 60, 30, WHITE);
  oled.setTextSize(2);
  oled.setCursor(40, 40);
  oled.println("PLAY");
  oled.display();
}

void drawGameOverScreen() {
  drawPoints(HEIGHT - 10);

  oled.setTextColor(WHITE);
  oled.setTextSize(2);
  oled.setCursor(10, 10);
  oled.println("GAME OVER");
  
  if(btnState == HIGH) {
    oled.fillRect(WIDTH * 0.5 - 15, 30, 30, 30, WHITE);

    drawTexture(redo_icon, NUM_ENTRIES(redo_icon), (WIDTH - 20) * 0.5, 35, 20, BLACK);
    oled.display();

    delay(200);
    isGameOverScreen = false;
    reset();

    return;
  }

  oled.drawRect(WIDTH * 0.5 - 15, 30, 30, 30, WHITE);
  drawTexture(redo_icon, NUM_ENTRIES(redo_icon), (WIDTH - 20) * 0.5, 35, 20, WHITE);
  oled.display();
}