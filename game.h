#ifndef GAME_H
#define GAME_H

/*Game*/
#define ROTSPEED 0.1
#define STEP_SIZE 0.2
#define SHOOT_BUTTON 10
#define RIGHT 0
#define LEFT 1

#define NBR_OF_ENEMIES 6

typedef struct Player {
  double x;
  double y;
  double dirX;
  double dirY;
  double planeX;
  double planeY;
  uint8_t shooting;
  uint8_t points;
} Player;

typedef struct enemy {
  int16_t xPos;
  int16_t yPos;
  uint8_t visible;
  uint8_t destroyed;
} Enemy;

void rotatePlayer(Player *player, uint8_t right);
void movePlayer(Player *player);
void disp_player_posistion (Player *player);
void drawSprite(Player *player, Enemy *enemy);
void playerShoot(Player *player);
void theEnd();
#endif /* GAME_H */