#include <tiny-fw.h>
#include <stdio.h>
#include "defs.h"

void player_display_status(player *p, int x, int y)
{
    char *status;
    char buffer[80];

    if (p->dir == UP)
        status = "UP";
    else if (p->dir == DOWN)
        status = "DOWN";
    else
        status = "STOP";

    snprintf(buffer, 80, "Player1:\nscore: %d\npos: %f, %f\ndir: %s\n", p->score, p->pos.e[X_COOR], p->pos.e[Y_COOR], status);
    draw_text(buffer, x, y, 0xffff00);
}

void player_init(player *p, COLLIDE_TYPE id, int score)
{
    p->score = score;
    p->pos.e[X_COOR] = 0;
    p->pos.e[Y_COOR] = 0;
    p->dir = STOP;
    p->id = id;
}

void player_update(player *p, App *app)
{
    switch (p->dir) {
        case UP:
            p->pos.e[Y_COOR] -= (PADDLE_SPEED * app->time.dt_sec);
            break;
        case DOWN:
            p->pos.e[Y_COOR] += (PADDLE_SPEED * app->time.dt_sec);
            break;
        default:
            break;
    }
}

void player_render(player *p)
{
    int x = p->pos.e[X_COOR];
    int y = p->pos.e[Y_COOR];

    draw_rect(x, y, x + PADDLE_WIDTH, y + PADDLE_HEIGHT, 0xffffff);
    player_display_status(p, 0, 0);
}

