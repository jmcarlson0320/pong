#include <tiny-fw.h>
#include "defs.h"

typedef struct {
    float x0;
    float y0;
    float x1;
    float y1;
} rect;

static int rect_collision(rect r0, rect r1);
static void check_wall_collisions(ball *b);
static void check_paddle_collisions(ball *b, player *p);

void check_collisions(pong *game)
{
    check_paddle_collisions(&game->b, &game->p1);
    check_wall_collisions(&game->b);
}

void process_collisions(pong *game)
{
    switch (game->b.collision) {
        case LEFT:
            game->b.pos.e[X_COOR] = BALL_SIZE;
            game->b.vel.e[X_COOR] *= (-1);
            break;
        case RIGHT:
            game->b.pos.e[X_COOR] = WIDTH - 1 - BALL_SIZE;
            game->b.vel.e[X_COOR] *= (-1);
            break;
        case TOP:
            game->b.pos.e[Y_COOR] = BALL_SIZE;
            game->b.vel.e[Y_COOR] *= (-1);
            break;
        case BOTTOM:
            game->b.pos.e[Y_COOR] = HEIGHT - 1 - BALL_SIZE;
            game->b.vel.e[Y_COOR] *= (-1);
            break;
        case PLAYER_1:
            game->b.pos.e[X_COOR] = PADDLE_WIDTH + BALL_SIZE;
            game->b.vel.e[X_COOR] *= (-1);
            break;
        default:
            break;
    }
    game->b.collision = NONE;
}

static int rect_collision(rect r0, rect r1)
{

    if (r0.x0 < r1.x1 && r0.x1 > r1.x0 && r0.y0 < r1.y1 && r0.y1 > r1.y0) {
        return 1;
    }
    return 0;
}

static void check_wall_collisions(ball *b)
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

static void check_paddle_collisions(ball *b, player *p)
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
