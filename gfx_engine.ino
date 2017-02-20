#include "gfx_engine.h"
#include "game.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

void drawLine(int16_t x, int16_t y0, int16_t y1) {
  while (y0>=y1) {
    display.drawPixel(x, y0, WHITE);
    y0--;
  }
}

void drawShadedLine(int16_t x, int16_t y0, int16_t y1) {
  uint16_t color = BLACK;
  uint8_t i = x%2;
  for (; y0>=y1; y0--) {
    i ? i=0:i=1;
    i ? color=WHITE:color=BLACK;
    display.drawPixel(x, y0, color);
  }
}

void drawWallLine(uint8_t x, uint8_t h, uint8_t side) {
  uint8_t drawStart = CAMERA_HEIGHT + (h >> 1);
  uint8_t drawEnd = CAMERA_HEIGHT - (h >> 1);
  if (drawStart < 0) {
    drawStart = 0;
  }
  if (drawEnd > DISP_HEIGHT) {
    drawEnd = DISP_HEIGHT;
  }
  if (side){
    drawLine(x, drawStart , drawEnd);
  } else {
    drawShadedLine(x, drawStart , drawEnd); //add shade to y-axis wall to give better contrast
  }
  drawFloorLine(x, drawStart);
}

void drawFloorLine(uint8_t x, uint8_t drawEnd) {
  drawLine(x, DISP_HEIGHT ,drawEnd + 2);
}

void doRayCasting(Player *player, Enemy *enemy) {
  for (uint8_t x = 0; x < DISP_WIDTH; x++) {
    //calculate ray position and direction
    double cameraX = 2 * x / (double)(DISP_WIDTH) - 1; //x-coordinate in camera space from -1 to 1
    double rayDirX = player->dirX + player->planeX * cameraX;
    double rayDirY = player->dirY + player->planeY * cameraX;
    // D(Serial.print("\nrayDirtX,y: "));
    // D(Serial.print(rayDirX, 3));
    // D(Serial.print(",  "));
    // D(Serial.println(rayDirY, 3));
    int mapX = (int)player->x;
    int mapY = (int)player->y;
    double deltaDistX = 0;
    double deltaDistY = 0;

    //length of ray from current position to next x or y-side
    double sideDistX = 0;
    double sideDistY = 0;

    // perpendicular distance to wall
    double perpWallDist = 0;

    //length of ray from one x or y-side to next x or y-side
    if (rayDirX != 0) {
      deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
    } else {
      deltaDistX = 99999;
    }

    if (rayDirY != 0) {
      deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
    } else {
      deltaDistY = 99999;
    }


    //what direction to step in x or y-direction (either +1 or -1)
    int stepX = 0;
    int stepY = 0;

    uint8_t hit = 0; //was there a wall hit?
    uint8_t side = 0; //was a NS or a EW wall hit?

    //calculate step and initial sideDist
    if (rayDirX < 0) {
      stepX = -1;
      sideDistX = (player->x - mapX) * deltaDistX;
    } else {
      stepX = 1;
      sideDistX = (mapX + 1.0 - player->x) * deltaDistX;
    }

    if (rayDirY < 0) {
      stepY = -1;
      sideDistY = (player->y - mapY) * deltaDistY;
    } else {
      stepY = 1;
      sideDistY = (mapY + 1 - player->y) * deltaDistY;
    }

    //DDA
    while (!hit) {
      if (sideDistX < sideDistY) {
        sideDistX += deltaDistX;
        mapX += stepX;
        side = 0;
      } else {
        sideDistY += deltaDistY;
        mapY += stepY;
        side = 1;
      }
      uint8_t mapData = pgm_read_byte(&level_map[mapY][mapX]);
      for (uint8_t i = 0; i < NBR_OF_ENEMIES; i++){
        if (!enemy[i].destroyed && enemy[i].xPos == mapX && enemy[i].yPos == mapY){
          enemy[i].visible = 1;
        }
      }
      // Check if ray has hit a wall
      mapData == 1 ? hit = 1 : hit = 0;

    }
    // D(Serial.print("map: "));
    // D(Serial.print(mapY));
    // D(Serial.print(", "));
    // D(Serial.print(mapX));
    // D(Serial.print(" - "));
    // D(Serial.println(level_map[mapY][mapX]));

    // Calculate distance to point of impact
    if (side == 0) {
      perpWallDist = sqrt(((mapX - player->x + (1 - stepX) / 2) / rayDirX) * ((mapX - player->x + (1 - stepX) / 2) / rayDirX));
    }
    else {
      perpWallDist = sqrt(((mapY - player->y + (1 - stepY) / 2) / rayDirY) * ((mapY - player->y + (1 - stepY) / 2) / rayDirY));
    }
    
    // D(Serial.print("\nperpWallDist: "));
    // D(Serial.println(perpWallDist, 3));
    
    uint8_t lineHeight;
    if (perpWallDist >= 1) { //values under 1 makes screen glitch
      lineHeight = abs((int)(DISP_HEIGHT / perpWallDist));
    }
    else {
      lineHeight = DISP_HEIGHT;
    }

    if (lineHeight >= 1) {
      drawWallLine(x, lineHeight, side);
    }
  }
}
