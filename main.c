#include <tiny-fw.h>
#include "defs.h"

void pong_init(pong *game)
{
    player_init(&game->p1, PLAYER_1, 999);
    ball_init(&game->b, 100, 100, 2, 1);
}

void pong_process_input(pong *game, App *app)
{
    if (app->keyboard.pressed[KEY_Q])
        app->running = 0;

    if (app->keyboard.down[KEY_UP])
        game->p1.dir = UP;
    else if (app->keyboard.down[KEY_DOWN])
        game->p1.dir = DOWN;
    else
        game->p1.dir = STOP;
}

void pong_update(pong *game, App *app)
{
    pong_process_input(game, app);
    player_update(&game->p1, app->time.dt_sec);
    ball_update(&game->b, app->time.dt_sec);
    check_collisions(game);
    process_collisions(game);
}

void pong_render(pong *game)
{
    draw_fill_rect(0, 0, WIDTH - 1, HEIGHT - 1, 0x000000);
    player_render(&game->p1);
    ball_render(&game->b);
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
