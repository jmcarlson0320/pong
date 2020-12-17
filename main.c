#include <tiny-fw.h>
#include <stdio.h>

#define SCALE_FACTOR 3
#define WIDTH (1920 / SCALE_FACTOR)
#define HEIGHT (1080 / SCALE_FACTOR)
#define PADDLE_SPEED 10
#define PADDLE_WIDTH 5
#define PADDLE_HEIGHT 20
#define BALL_SPEED 10

typedef enum {
    UP,
    DOWN,
    STOP
} STATUS;

typedef struct {
    int score;
    point2 pos;
    STATUS dir;
} player;

typedef struct {
    point2 pos;
    vec2 vel;
} ball;

typedef struct {
    player p1;
    player p2;
    ball b;
} pong;

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

    snprintf(buffer, 80, "score: %d\npos: %f, %f\ndir: %s\n", p->score, p->pos.e[X_COOR], p->pos.e[Y_COOR], status);
    draw_text(buffer, x, y, 0xffff00);
}

void player_init(player *p, int score)
{
    p->score = score;
    p->pos.e[X_COOR] = 0;
    p->pos.e[Y_COOR] = 0;
    p->dir = STOP;
}

void player_update(player *p, App *app)
{
    if (app->keyboard.down[KEY_UP])
        p->dir = UP;
    else if (app->keyboard.down[KEY_DOWN])
        p->dir = DOWN;
    else
        p->dir = STOP;
}

void player_render(player *p)
{
    player_display_status(p, 50, 50);
}

void pong_init(pong *game)
{
    player_init(&game->p1, 100);
}

void pong_update(pong *game, App *app)
{
    if (app->keyboard.pressed[KEY_Q])
        app->running = 0;
    player_update(&game->p1, app);
}

void pong_render(pong *game)
{
    draw_fill_rect(0, 0, WIDTH - 1, HEIGHT - 1, 0x000000);
    draw_text("pong!!!\n"
              "press 'Q' to quit", 0, 0, 0x00ffff);
    player_render(&game->p1);
}

int main(int argc, char *argv[])
{
    App app = app_create(WIDTH, HEIGHT);
    pong game;
    Bitmap canvas = bitmap_create(app.graphics.width, app.graphics.height, app.graphics.pixels_rgb);

    graphics_output_set(&canvas);
    pong_init(&game);
    app_start(&app);
    while (app.running) {
        app_update(&app);

        pong_update(&game, &app);
        pong_render(&game);

        app_draw_graphics_to_screen(&app);
    }
    app_quit(&app);

    return 0;
}
