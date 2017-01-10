#ifndef GAME_H
#define GAME_H

/*Game*/
#define ROTSPEED 0.1
#define STEP_SIZE 0.2
#define UP_BUTTON 10
#define LEFT_BUTTON 9
#define RIGHT_BUTTON 11
#define RIGHT 0
#define LEFT 1

typedef struct Player {
  double x;
  double y;
  double dirX;
  double dirY;
  double planeX;
  double planeY;
} Player;

void rotatePlayer(Player *player, uint8_t right);
void movePlayer(Player *player);
void disp_player_posistion (Player *player);

#endif /* GAME_H */