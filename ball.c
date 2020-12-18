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

void ball_update(ball *b, App *app)
{
    vec2 tmp = b->vel;
    vec2_mult(&tmp, &tmp, app->time.dt_sec);
    vec2_add(&b->pos, &b->pos, &tmp);
}

void ball_render(ball *b)
{
    draw_circle(b->pos.e[X_COOR], b->pos.e[Y_COOR], BALL_SIZE, 0xffffff);
}

int rect_collision(rect r0, rect r1)
{

    if (r0.x0 < r1.x1 && r0.x1 > r1.x0 && r0.y0 < r1.y1 && r0.y1 > r1.y0) {
        return 1;
    }
    return 0;
}

void ball_check_wall_collisions(ball *b)
{
    float x = b->pos.e[X_COOR];
    float y = b->pos.e[Y_COOR];
    if (x < BALL_SIZE)
        b->collision = LEFT;
    if (x > WIDTH - 1 - BALL_SIZE)
        b->collision = RIGHT;
    if (y < BALL_SIZE)
        b->collision = TOP;
    if (y > HEIGHT - 1 - BALL_SIZE)
        b->collision = BOTTOM;
}

void ball_check_paddle_collisions(ball *b, player *p)
{
    rect ball_rect = {
        b->pos.e[X_COOR] - BALL_SIZE,
        b->pos.e[Y_COOR] - BALL_SIZE,
        b->pos.e[X_COOR] + BALL_SIZE,
        b->pos.e[Y_COOR] + BALL_SIZE
    };

    rect player_rect = {
        p->pos.e[X_COOR],
        p->pos.e[Y_COOR],
        p->pos.e[X_COOR] + PADDLE_WIDTH,
        p->pos.e[Y_COOR] + PADDLE_HEIGHT
    };

    if (rect_collision(ball_rect, player_rect))
        b->collision = p->id;
}

void ball_process_collision(ball *b)
{
    switch (b->collision) {
        case LEFT:
            b->pos.e[X_COOR] = BALL_SIZE;
            b->vel.e[X_COOR] *= (-1);
            break;
        case RIGHT:
            b->pos.e[X_COOR] = WIDTH - 1 - BALL_SIZE;
            b->vel.e[X_COOR] *= (-1);
            break;
        case TOP:
            b->pos.e[Y_COOR] = BALL_SIZE;
            b->vel.e[Y_COOR] *= (-1);
            break;
        case BOTTOM:
            b->pos.e[Y_COOR] = HEIGHT - 1 - BALL_SIZE;
            b->vel.e[Y_COOR] *= (-1);
            break;
        case PLAYER_1:
            b->pos.e[X_COOR] = PADDLE_WIDTH + BALL_SIZE;
            b->vel.e[X_COOR] *= (-1);
            break;
        default:
            break;
    }
    b->collision = NONE;
}
