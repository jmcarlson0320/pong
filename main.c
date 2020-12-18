#include <tiny-fw.h>
#include <stdio.h>

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

int rect_collision(rect r0, rect r1)
{

    if (r0.x0 < r1.x1 && r0.x1 > r1.x0 && r0.y0 < r1.y1 && r0.y1 > r1.y0) {
        return 1;
    }
    return 0;
}

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

void pong_check_collisions(pong *game)
{
    ball_check_paddle_collisions(&game->b, &game->p1);
    ball_check_wall_collisions(&game->b);
}

void pong_init(pong *game)
{
    player_init(&game->p1, PLAYER_1, 999);
    ball_init(&game->b, 100, 100, 2, 1);
}

void pong_update(pong *game, App *app)
{
    pong_process_input(game, app);
    player_update(&game->p1, app);
    ball_update(&game->b, app);
    pong_check_collisions(game);
    ball_process_collision(&game->b);
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
