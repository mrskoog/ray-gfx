#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define D(x)

#define I2C_ADDRESS 0x3C
#define OLED_RESET 3 //not used but needed by lib
Adafruit_SSD1306 display(OLED_RESET);

/*gfx engine*/
#define CAMERA_HEIGHT	32
#define DISP_WIDTH	128
#define DISP_HEIGHT	64
#define VIEW_RANGE 8



/*Game*/
#define ROTSPEED 0.045

const uint8_t level_map[][9] =
{
  {1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1}
};

typedef struct Player {
  double x;
  double y;
  double dirX;
  double dirY;
  double planeX;
  double planeY;
} Player;

void doRayCasting(Player *player);
void drawWallLine(uint8_t x, uint8_t h);
void rotatePlayer(Player *player, uint8_t right);

void setup()  {
  D(Serial.begin(9600));

  display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay(); //clear adafruit logo from buffer
  display.display(); //init display
}

void loop() {
  Player player;
  player.x = 4;
  player.y = 3;
  player.dirX = -1;
  player.dirY = 0;
  player.planeX = 0;
  player.planeY = 0.66;

  while (1) {
    display.clearDisplay();
    doRayCasting(&player);
    display.display();
    rotatePlayer(&player, 0);
  }
}

void rotatePlayer(Player *player, uint8_t right){
    double rotation = right? -ROTSPEED : ROTSPEED;

    double oldXDir = player->dirX;
    player->dirX = player->dirX * cos(rotation) - player->dirY * sin(rotation);
    player->dirY = oldXDir * sin(rotation) + player->dirY * cos(rotation);

    double oldplaneX = player->planeX;
    player->planeX = player->planeX * cos(rotation) - player->planeY * sin(rotation);
    player->planeY = oldplaneX * sin(rotation) + player->planeY * cos(rotation);
}

void drawWallLine(uint8_t x, uint8_t h) {
  uint8_t drawStart = CAMERA_HEIGHT + (h >> 1);
  uint8_t drawEnd = CAMERA_HEIGHT - (h >> 1);
  if (drawStart < 0) {
    drawStart = 0;
  }
  if (drawEnd > DISP_HEIGHT) {
    drawEnd = DISP_HEIGHT;
  }

  display.drawLine(x, drawStart , x, drawEnd, WHITE);
}

void doRayCasting(Player *player) {
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

    double perpWallDist = 0;

    // D(Serial.print("\ndeltaDistX,y: "));
    // D(Serial.print(deltaDistX, 3));
    // D(Serial.print(",  "));
    // D(Serial.println(deltaDistY, 3));

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

    uint8_t i = 0;
    //DDA
    while (!hit ) {
      if (sideDistX < sideDistY) {
        sideDistX += deltaDistX;
        mapX += stepX;
        side = 0;
      } else {
        sideDistY += deltaDistY;
        mapY += stepY;
        side = 1;
      }

      // Check if ray has hit a wall
      level_map[mapY][mapX] != 0 ? hit = 1 : hit = 0;

      i++;
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
    if (perpWallDist > 0) {
      lineHeight = abs((int)(DISP_HEIGHT / perpWallDist));
    }
    else {
      lineHeight = DISP_HEIGHT;
    }

    if (lineHeight >= 1) {
      drawWallLine(x, lineHeight);
    }
  }
}

