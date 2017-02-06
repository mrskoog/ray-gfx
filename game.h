#ifndef GAME_H
#define GAME_H

/*Game*/
#define ROTSPEED 0.1
#define STEP_SIZE 0.2
#define SHOOT_BUTTON 10
#define RIGHT 0
#define LEFT 1

#define NBR_OF_TARGETS 2

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

typedef struct target {
  int16_t xPos;
  int16_t yPos;
  uint8_t visible;
  uint8_t destroyed;
} Target;

void rotatePlayer(Player *player, uint8_t right);
void movePlayer(Player *player);
void disp_player_posistion (Player *player);
void drawTarget(Player *player, Target *target);
void playerShoot(Player *player);
#endif /* GAME_H */