#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimerOne.h>
#include "game.h"
#include "gfx_engine.h"

#define D(x)

#define I2C_ADDRESS 0x3C
#define OLED_RESET 3 //not used but needed by lib
Adafruit_SSD1306 display(OLED_RESET);

void timer_IRQ();
void drawFrame(Enemy *enemy, Player *player);
void resetEnemys(Enemy *enemy);

volatile uint8_t timer_flag = 0;

void setup() {
  D(Serial.begin(9600));

  pinMode(SHOOT_BUTTON, INPUT_PULLUP);

  Timer1.initialize(50000); 
  Timer1.attachInterrupt(timer_IRQ);

  display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay();
  display.display(); //init display
}

void drawFrame(Enemy *enemy, Player *player) {
  display.clearDisplay();
  resetEnemys(enemy);
  doRayCasting(player, enemy);
  drawSprite(player, enemy);
  drawHUD(player);
  if (player->points == NBR_OF_ENEMIES) {
    theEnd();
  }
  display.display();
}

void resetEnemys(Enemy *enemy) {
  for (uint8_t i = 0; i < NBR_OF_ENEMIES; i++) {
    enemy[i].visible = 0;
  }
}

void loop() {
  //New player
  Player player;
  player.x = 2;
  player.y = 3;
  player.dirX = 1;
  player.dirY = 0;
  player.planeX = 0;
  player.planeY = 0.66;
  player.shooting = 0;
  player.points = 0;

  Enemy enemy[NBR_OF_ENEMIES]  {
    {2, 7, 0, 0},
    {6, 3, 0, 0},
    {2, 11, 0, 0},
    {11, 22, 0, 0},
    {21, 17, 0, 0},
    {32, 32, 0, 0}
  };

  while (1) {
    if (timer_flag) {
      movePlayer(&player);
      playerShoot(&player);
      drawFrame(&enemy[0], &player);

      player.shooting = 0;
      timer_flag = 0;
    }
  }
}

void timer_IRQ(){
  timer_flag = 1;
}

