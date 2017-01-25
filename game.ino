#include "gfx_engine.h"
#include "game.h"
#include <avr/pgmspace.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdlib.h>
#include <stdio.h>

const uint8_t level_map[][35] PROGMEM =
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
  {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
  {1,0,0,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,1,1,1,1,1,0,0,1,0,0,0,1,0,0,0,1},
  {1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,1},
  {1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1},
  {1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1},
  {1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,1,0,0,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

const uint8_t gunBitmap [] PROGMEM = {
0x01, 0x80, 0x01, 0x80, 0x1f, 0xf8, 0x20, 0x04, 0x2f, 0xf4, 0x2f, 0xf4, 0x2f, 0xf4, 0x2f, 0xf4,
0x2f, 0xf4, 0x1f, 0xf8, 0x48, 0x08, 0xb7, 0xfe, 0x90, 0x02, 0x90, 0x01, 0x80, 0x02, 0x80, 0x01,
};

void disp_player_posistion (Player *player) {
  int n = 0;
  char buffer [10];
  n = snprintf (buffer, 12, "%d:%d", (int)player->x, (int)player->y);
  n--;
  while(n >= 0){
    display.drawChar(1+n*6, 56, buffer[n], BLACK, WHITE, 1);
    n--;
  }
}

void drawHUD() {
  display.drawBitmap((DISP_WIDTH/2) - 6, DISP_HEIGHT-16, gunBitmap, 16, 16, BLACK);
}

void drawTarget(Player *player, Target *target) {
  for (uint8_t i  = 0; i < NBR_OF_TARGETS; i++) {
    if (target[i].visible) {
      //translate sprite position to relative to camer
      double spriteX = target[i].xPos - player->x;
      double spriteY = target[i].yPos - player->y;

      //transform sprite with the inverse camera matrix (projection in camera plain)
      // [ planeX   dirX ] -1                                       [ dirY      -dirX ]
      // [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
      // [ planeY   dirY ]                                          [ -planeY  planeX ]

      double invDet = 1.0 / (player->planeX * player->dirY - player->dirX * player->planeY); //required for correct matrix multiplication

      double transformX = invDet * (player->dirY * spriteX - player->dirX * spriteY);
      double transformY = invDet * (-1*player->planeY * spriteX + player->planeX * spriteY);

      int16_t spriteScreenX = int((DISP_WIDTH / 2) * (1 + transformX / transformY));

      int16_t spriteHeight = abs(int(DISP_HEIGHT / (transformY)));

      display.fillCircle(spriteScreenX, CAMERA_HEIGHT, spriteHeight >> 3, BLACK);
    }
  }
}

void rotatePlayer(Player *player, uint8_t right) {
    double rotation = right? -ROTSPEED : ROTSPEED;

    double oldXDir = player->dirX;
    player->dirX = player->dirX * cos(rotation) - player->dirY * sin(rotation);
    player->dirY = oldXDir * sin(rotation) + player->dirY * cos(rotation);

    double oldplaneX = player->planeX;
    player->planeX = player->planeX * cos(rotation) - player->planeY * sin(rotation);
    player->planeY = oldplaneX * sin(rotation) + player->planeY * cos(rotation);
}

void movePlayer(Player *player) {
  if ( analogRead(A1) < 200) {
    double nextStepX = player->dirX * STEP_SIZE;
    double nextStepY = player->dirY * STEP_SIZE;

    if (pgm_read_byte(&level_map[int(player->y)][int(player->x + nextStepX)]) != 1) {
        player->x += nextStepX;
     } 

    if (pgm_read_byte(&level_map[int(player->y + nextStepY)][int(player->x)]) != 1) {
        player->y += nextStepY;
     }
  }

    if (analogRead(A1) > 700) {
    double nextStepX = player->dirX * STEP_SIZE;
    double nextStepY = player->dirY * STEP_SIZE;

    if (pgm_read_byte(&level_map[int(player->y)][int(player->x + nextStepX)]) != 1) {
        player->x -= nextStepX;
     }

    if (pgm_read_byte(&level_map[int(player->y + nextStepY)][int(player->x)]) != 1) {
        player->y -= nextStepY;
     }
  }

  if (analogRead(A0) > 700) {
    rotatePlayer(player, LEFT);
  }

  if (analogRead(A0) < 200) {
    rotatePlayer(player, RIGHT);
  }
}

