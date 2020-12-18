#include <tiny-fw.h>
#include <stdio.h>
#include "defs.h"

void player_display_status(player *p, int x, int y)
{
    char *status;
    char *player;
    char buffer[80];

    if (p->dir == UP)
        status = "UP";
    else if (p->dir == DOWN)
        status = "DOWN";
    else
        status = "STOP";

    if (p->id == PLAYER_1)
        player = "Player1";
    else
        player = "Player2";

    snprintf(buffer, 80, "%s:\nscore: %d\npos: %f, %f\ndir: %s\n", player, p->score, p->pos.e[X_COOR], p->pos.e[Y_COOR], status);
    draw_text(buffer, x, y, 0xffff00);
}

void player_init(player *p, COLLIDE_TYPE id, int score)
{
    p->id = id;
    p->score = score;
    if (p->id == PLAYER_1) {
        p->pos.e[X_COOR] = 0;
        p->pos.e[Y_COOR] = 0;
    } else if (p->id == PLAYER_2) {
        p->pos.e[X_COOR] = WIDTH - 1 - PADDLE_WIDTH;
        p->pos.e[Y_COOR] = 0;
    }
        p->dir = STOP;
}

void player_update(player *p, float dt)
{
    switch (p->dir) {
        case UP:
            p->pos.e[Y_COOR] -= (PADDLE_SPEED * dt);
            break;
        case DOWN:
            p->pos.e[Y_COOR] += (PADDLE_SPEED * dt);
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
    if (p->id == PLAYER_1)
        player_display_status(p, 0, 0);
    else
        player_display_status(p, 283, 0);
        
}
