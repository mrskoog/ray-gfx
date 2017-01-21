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

volatile uint8_t timer_flag = 0;

void setup()  {
  D(Serial.begin(9600));

  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);

  Timer1.initialize(50000); 
  Timer1.attachInterrupt(timer_IRQ);

  display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay();
  display.display(); //init display
}

void loop() {
  Player player;
  player.x = 2;
  player.y = 3;
  player.dirX = 1;
  player.dirY = 0;
  player.planeX = 0;
  player.planeY = 0.66;
  display.clearDisplay();
  doRayCasting(&player);
  display.display();
  while (1) {
    if (timer_flag) {
      display.clearDisplay();
      movePlayer(&player);
      doRayCasting(&player);
      disp_player_posistion(&player);
      drawHUD();
      display.display();
      timer_flag = 0;
    }
  }
}

void timer_IRQ(){
  timer_flag = 1;
}

