#include <tiny-fw.h>
#include <stdio.h>

#define SCALE_FACTOR 4
#define WIDTH (1920 / SCALE_FACTOR)
#define HEIGHT (1080 / SCALE_FACTOR)
#define PADDLE_SPEED 100
#define PADDLE_WIDTH 5
#define PADDLE_HEIGHT 20
#define BALL_SPEED 200
#define BALL_SIZE 2

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
    PADDLE_1,
    PADDLE_2,
    NONE
} COLLIDE_TYPE;

typedef struct {
    int score;
    point2 pos;
    STATUS dir;
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

void player_init(player *p, int score)
{
    p->score = score;
    p->pos.e[X_COOR] = 0;
    p->pos.e[Y_COOR] = 0;
    p->dir = STOP;
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
    
    // make a method: process_collision(ball *b);
    //
    // ends w/ b->collision = NONE;
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
        default:
            break;
    }
    b->collision = NONE;
}

void ball_render(ball *b)
{
    draw_circle(b->pos.e[X_COOR], b->pos.e[Y_COOR], BALL_SIZE, 0xffffff);
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

void pong_init(pong *game)
{
    player_init(&game->p1, 999);
    ball_init(&game->b, 100, 100, 2, 1);
}

void pong_update(pong *game, App *app)
{
    pong_process_input(game, app);
    player_update(&game->p1, app);
    ball_update(&game->b, app);

    // make a method: check_ball_wall_collisions(pong *game)
    float x = game->b.pos.e[X_COOR];
    float y = game->b.pos.e[Y_COOR];
    if (x < BALL_SIZE)
        game->b.collision = LEFT;
    if (x > WIDTH - 1 - BALL_SIZE)
        game->b.collision = RIGHT;
    if (y < BALL_SIZE)
        game->b.collision = TOP;
    if (y > HEIGHT - 1 - BALL_SIZE)
        game->b.collision = BOTTOM;
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
