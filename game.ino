#include "gfx_engine.h"
#include "game.h"
#include <avr/pgmspace.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdlib.h>
#include <stdio.h>

#define D(x) x

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
  n = snprintf (buffer, 10, "%d:%d", (int)player->x, (int)player->y);
  n--;
  while(n >= 0){
    display.drawChar(1+n*6, 56, buffer[n], BLACK, WHITE, 1);
    n--;
  }
}

void drawHUD(Player *player) {
  char buffer [5];
  int n = 0;

  display.drawBitmap((DISP_WIDTH/2) - 8, DISP_HEIGHT-16, gunBitmap, 16, 16, BLACK);

  if (player->shooting) {
    display.fillRect((DISP_WIDTH/2) - 4, DISP_HEIGHT-12, 8, 5, WHITE);
  }

  disp_player_posistion(player);
  //draw score
  n = snprintf (buffer, 5, "P:%d", (int)player->points);
  n--;
  while(n >= 0){
    display.drawChar(98+n*6, 56, buffer[n], BLACK, WHITE, 1);
    n--;
  }
}

/*Sets player in shooting state.
  Collision detection happens in drawSprite function*/
void playerShoot(Player *player) {
  if (digitalRead(SHOOT_BUTTON) == 0 || digitalRead(SHOOT_BUTTON2) == 0) {
    player->shooting = 1;
  }
}

 /*limitation: to reduce calculations no enemy sorting is done -> only one enemy can be visible at a time*/
void drawSprite(Player *player, Enemy *enemy) {
  for (uint8_t i  = 0; i < NBR_OF_ENEMIES; i++) {
    if (enemy[i].visible && !enemy[i].destroyed) {
      //translate sprite position to relative to camera
      double spriteX = enemy[i].xPos - player->x;
      double spriteY = enemy[i].yPos - player->y;

      //transform sprite with the inverse camera matrix (projection in camera plain)
      // [ planeX   dirX ] -1                                       [ dirY      -dirX ]
      // [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
      // [ planeY   dirY ]                                          [ -planeY  planeX ]

      double invDet = 1.0 / (player->planeX * player->dirY - player->dirX * player->planeY); //required for correct matrix multiplication

      double transformX = invDet * (player->dirY * spriteX - player->dirX * spriteY);
      double transformY = invDet * (-1*player->planeY * spriteX + player->planeX * spriteY);

      int16_t spriteScreenX = int((DISP_WIDTH / 2) * (1 + transformX / transformY));

      int16_t spriteHeight = abs(int(DISP_HEIGHT / (transformY)));

      //check i center screen is inside enemy
      if (player->shooting && DISP_WIDTH/2 >= (spriteScreenX - (spriteHeight >> 2))  && 
          DISP_WIDTH/2 <= (spriteScreenX + (spriteHeight >> 2))) {
        //play shooting sound
        enemy[i].destroyed = 1;
        player->points += 1;
                display.drawCircle(spriteScreenX, CAMERA_HEIGHT, spriteHeight >> 2, WHITE);
        display.fillCircle(spriteScreenX, CAMERA_HEIGHT, (spriteHeight >> 2) - 1, BLACK);
        display.fillCircle(spriteScreenX + (spriteHeight >> 3), CAMERA_HEIGHT - (spriteHeight >> 4), spriteHeight >> 4, WHITE);
        display.fillCircle(spriteScreenX - (spriteHeight >> 3), CAMERA_HEIGHT - (spriteHeight >> 4), spriteHeight >> 4, WHITE);
        display.drawLine(spriteScreenX + (spriteHeight >> 4), CAMERA_HEIGHT + (spriteHeight >> 3), spriteScreenX - (spriteHeight >> 4), CAMERA_HEIGHT + (spriteHeight >> 3), WHITE);
      } else {
        display.drawCircle(spriteScreenX, CAMERA_HEIGHT, spriteHeight >> 2, BLACK);
        display.fillCircle(spriteScreenX, CAMERA_HEIGHT, (spriteHeight >> 2) - 1, WHITE);
        display.fillCircle(spriteScreenX + (spriteHeight >> 3), CAMERA_HEIGHT - (spriteHeight >> 4), spriteHeight >> 4, BLACK);
        display.fillCircle(spriteScreenX - (spriteHeight >> 3), CAMERA_HEIGHT - (spriteHeight >> 4), spriteHeight >> 4, BLACK);
        display.drawLine(spriteScreenX + (spriteHeight >> 4), CAMERA_HEIGHT + (spriteHeight >> 3), spriteScreenX - (spriteHeight >> 4), CAMERA_HEIGHT + (spriteHeight >> 3), BLACK);
      }
      return; //only one enemy visible a time
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
  double nextStepX = player->dirX * STEP_SIZE;
  double nextStepY = player->dirY * STEP_SIZE;

  if ( analogRead(A1) > 200) {

    if (pgm_read_byte(&level_map[int(player->y)][int(player->x + nextStepX)]) != 1) {
        player->x += nextStepX;
     } 

    if (pgm_read_byte(&level_map[int(player->y + nextStepY)][int(player->x)]) != 1) {
        player->y += nextStepY;
     }
  }

    if (analogRead(A1) < 700) {

    if (pgm_read_byte(&level_map[int(player->y)][int(player->x - nextStepX)]) != 1) {
        player->x -= nextStepX;
     }

    if (pgm_read_byte(&level_map[int(player->y - nextStepY)][int(player->x)]) != 1) {
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

void theEnd() {
  display.setCursor(24,12);
  display.setTextColor(BLACK, WHITE);
  display.setTextSize(2);
  display.print("MISSION");
  display.setCursor(12,30);
  display.print("COMPLETED");
}

