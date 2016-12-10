#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <avr/pgmspace.h>

#define D(x) x

#define I2C_ADDRESS 0x3C
#define OLED_RESET 3 //not used but needed by lib
Adafruit_SSD1306 display(OLED_RESET);

/*gfx engine*/
#define CAMERA_HEIGHT	32
#define DISP_WIDTH	128
#define DISP_HEIGHT	64



/*Game*/
#define ROTSPEED 0.78

const uint8_t level_map[][8] PROGMEM =
{
  {1,1,1,1,1,1,1,1},
  {1,0,1,0,0,0,0,1},
  {1,0,1,0,0,0,0,1},
  {1,0,1,0,0,0,0,1},
  {1,0,1,0,0,0,0,1},
  {1,0,0,0,1,1,1,1},
  {1,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1}
};

typedef struct Player {
	float x;
	float y;
	float dirX;
	float dirY;
	float planeX;
	float planeY;
} Player;

void doRayCasting(Player *player);
void drawWallLine(uint8_t x,uint8_t h);

void printDouble( float val, uint8_t precision){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

   Serial.print (int(val));  //prints the int part
   Serial.print("."); // print the decimal point
   unsigned int frac;
   if(val >= 0)
       frac = (val - int(val)) * precision;
   else
       frac = (int(val)- val ) * precision;
   Serial.println(frac,DEC) ;
}

void setup()  {
	D(Serial.begin(9600));
	
	display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS);  // initialize with the I2C addr 0x3C (for the 128x64)
	display.clearDisplay(); //clear adafruit logo from buffer
	display.display(); //init display
}


void loop() {
	Player player;
	player.x = 7;
	player.y = 7;
	player.dirX = -1;
	player.dirY = 0;
	player.planeX = 0;
	player.planeY = 0.66;

	int bla = 0;

	while(1){
		D(Serial.print("\n\nframe\n"));
		D(Serial.print(bla));
		bla = (bla +1) % 8;
		display.clearDisplay();
		doRayCasting(&player);
		display.display();
		delay(10000);

		float oldXDir = player.dirX;
		player.dirX = player.dirX*cos(-ROTSPEED) - player.dirY*sin(-ROTSPEED);
		player.dirY = oldXDir*sin(-ROTSPEED) + player.dirY*cos(-ROTSPEED);

		double oldplaneX = player.planeX;
		player.planeX = player.planeX*cos(-ROTSPEED) - player.planeY*sin(-ROTSPEED);
		player.planeY = oldplaneX*sin(-ROTSPEED) + player.planeY*cos(-ROTSPEED);
	}

}

void drawWallLine(uint8_t x,uint8_t h) {
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
		float cameraX = 2 * x / (float)DISP_WIDTH - 1; //x-coordinate in camera space
		float rayPosX = player->x;
		float rayPosY = player->y;
		float rayDirX = player->dirX + player->planeX * cameraX;
		float rayDirY = player->dirY + player->planeY * cameraX;

		int mapX = (int)player->x;
		int mapY = (int)player->y;

		//length of ray from current position to next x or y-side
		float sideDistX;
		float sideDistY;

		//length of ray from one x or y-side to next x or y-side
		float deltaDistX = 0.1; //sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
		float deltaDistY = 0.1; //sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
		float perpWallDist;

		//what direction to step in x or y-direction (either +1 or -1)
		uint8_t stepX;
		uint8_t stepY;

		uint8_t hit = 0; //was there a wall hit?
		uint8_t side; //was a NS or a EW wall hit?

		//calculate step and initial sideDist
		if (rayDirX < 0) {
			stepX = -1;
			sideDistX = (player->x - mapX) * deltaDistX;
		} else {
			stepX = 1;
			sideDistX = (mapX + 1.0 - player->x) * deltaDistX;
		}

		if(rayDirY < 0) {
			stepY = -1;
			sideDistY = (player->y - mapY) * deltaDistY;
		} else {
			stepY = 1;
			sideDistY = (mapY + 1 - player->y) * deltaDistY;
		}

		//DDA
		while(!hit) {
			if(sideDistX < sideDistY) {
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
		}

		if(side) {
			perpWallDist = sqrt(((mapY - player->y + (1 - stepY) / 2) / rayDirY) * ((mapY - player->y + (1 - stepY) / 2) / rayDirY));
		}
		else {
			perpWallDist = sqrt(((mapX - player->x + (1 - stepX) / 2) / rayDirX) * ((mapX - player->x + (1 - stepX) / 2) / rayDirX));
		}
		Serial.print("dist");
		printDouble(perpWallDist,3);
		uint8_t lineHeight;
		if(perpWallDist > 0) {
			lineHeight = abs((int)(DISP_HEIGHT / (perpWallDist / 100)));
		}
		else {
			lineHeight = DISP_HEIGHT;
		}
		if (lineHeight >= 1) {
			drawWallLine(x,lineHeight);
		}
	}
}

