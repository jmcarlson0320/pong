#include <tiny-fw.h>
#include "defs.h"

void ball_init(ball *b, int x, int y, int dx, int dy)
{
    b->pos.e[X_COOR] = x;
    b->pos.e[Y_COOR] = y;
    b->vel.e[X_COOR] = dx;
    b->vel.e[Y_COOR] = dy;
    vec2_normalize(&b->vel, &b->vel);
    vec2_mult(&b->vel, &b->vel, BALL_SPEED);
    b->collision = NONE;
}

void ball_update(ball *b, float dt)
{
    vec2 tmp = b->vel;
    vec2_mult(&tmp, &tmp, dt);
    vec2_add(&b->pos, &b->pos, &tmp);
}

void ball_render(ball *b)
{
    draw_circle(b->pos.e[X_COOR], b->pos.e[Y_COOR], BALL_SIZE, 0xffffff);
}
