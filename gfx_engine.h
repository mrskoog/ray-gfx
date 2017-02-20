#ifndef GFXENGINE_H
#define GFXENGINE_H

void doRayCasting(Player *player, Enemy *enemy);
void drawWallLine(uint8_t x, uint8_t h, uint8_t side);
void drawLine(int16_t x, int16_t y0, int16_t y1);
void drawShadedLine(int16_t x, int16_t y0, int16_t y1);

/*gfx engine*/
#define CAMERA_HEIGHT	32
#define DISP_WIDTH	128
#define DISP_HEIGHT	64
#define VIEW_RANGE 8
#endif /* GFXENGINE_H_H */