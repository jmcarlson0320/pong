#ifndef DEFS_H
#define DEFS_H

#include <tiny-fw.h>

#define SCALE_FACTOR 4
#define WIDTH (1920 / SCALE_FACTOR)
#define HEIGHT (1080 / SCALE_FACTOR)
#define PADDLE_SPEED 150
#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 40
#define BALL_SPEED 100
#define BALL_SIZE 3

typedef enum {
    UP,
    DOWN,
    STOP
} STATUS;

typedef enum {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    PLAYER_1,
    PLAYER_2,
    NONE
} COLLIDE_TYPE;

typedef struct {
    int score;
    point2 pos;
    STATUS dir;
    COLLIDE_TYPE id;
} player;

typedef struct {
    point2 pos;
    vec2 vel;
    COLLIDE_TYPE collision;
} ball;

typedef struct {
    player p1;
    player p2;
    ball b;
} pong;

typedef struct {
    float x0;
    float y0;
    float x1;
    float y1;
} rect;

// main.c
void pong_init(pong *game);
void pong_update(pong *game, App *app);
void pong_render(pong *game);
void pong_process_input(pong *game, App *app);
void pong_check_collisions(pong *game);

// player.c
void player_init(player *p, COLLIDE_TYPE id, int score);
void player_update(player *p, App *app);
void player_render(player *p);
void player_display_status(player *p, int x, int y);

// ball.c
void ball_init(ball *b, int x, int y, int dx, int dy);
void ball_update(ball *b, App *app);
void ball_render(ball *b);
int rect_collision(rect r0, rect r1);
void ball_check_wall_collisions(ball *b);
void ball_check_paddle_collisions(ball *b, player *p);
void ball_process_collision(ball *b);

#endif // DEFS_H