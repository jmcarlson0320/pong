#include <tiny-fw.h>

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

void player_update(player *p, App *app);
void player_render(player *p);

void pong_init(pong *game)
{
    return;
}

void pong_update(pong *game, App *app)
{
    if (app->keyboard.pressed[KEY_Q])
        app->running = 0;
}

void pong_render(pong *game)
{
    draw_text("pong!!!\n"
              "press 'Q' to quit", 0, 0, 0x00ffff);
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
