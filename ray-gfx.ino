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
const uint8_t level_map[][8] PROGMEM =
{
  {1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,1},
  {1,1,1,1,0,0,0,1},
  {1,0,0,0,0,0,0,1},
  {1,0,0,0,0,1,0,1},
  {1,0,0,0,0,1,0,1},
  {1,0,0,0,0,1,0,1},
  {1,1,1,1,1,1,1,1}
};

struct Player {
	float x;
	float y;
	float dirX;
	float dirY;
	float planeX;
	float planeY;
}

void setup()  {
	D(Serial.begin(9600));
	
	display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS);  // initialize with the I2C addr 0x3C (for the 128x64)
	display.clearDisplay(); //clear adafruit logo from buffer
	display.display(); //init display
}


void loop() {
	struct Player player;
	player.x = 1;
	player.y = 1;
	player.dirX = -1;
	player.dirY = 0;
	player.planeX = 0;
	player.planeY = 0.66;


	for (uint8_t x = 0; x < DISP_WIDTH; x++) {
		drawWallLine(x, abs(x - 64));
	}
	display.display();

	while(1){
		// statement
	}

}

void drawWallLine(uint8_t x,uint8_t h) {
	display.drawLine(x, CAMERA_HEIGHT + (h >> 1) , x, CAMERA_HEIGHT - (h >> 1), WHITE);
}

void doRayCasting(Player player) {
	for (uint8_t x = 0; x < DISP_WIDTH; x++) {
		//calculate ray position and direction
		float cameraX = 2 * x / (float)DISP_WIDTH - 1; //x-coordinate in camera space
		float rayPosX = player.x;
		float rayPosY = player.y;
		float rayDirX = player.dirX + player.planeX * cameraX;
		float rayDirY = player.dirY + player.planeY * cameraX;

		int mapX = (int)player.x;
		int mapY = (int)player.y;

		//length of ray from current position to next x or y-side
		double sideDistX;
		double sideDistY;

		//length of ray from one x or y-side to next x or y-side
		double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
		double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
		double perpWallDist;

	}
}









