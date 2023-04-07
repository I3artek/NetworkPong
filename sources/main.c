#include "raylib.h"

#include "game.h"
#include "nettools.h"

#define WINDOW_TITLE "Window title"
#define CONNECT_BTN_WIDTH 400
#define CONNECT_BTN_HEIGHT 100

struct  game_info
{
    float   upper_paddle_x;
    float   lower_paddle_x;
    Vector2 ball_pos;
};

void    draw_ball_and_paddles(struct game_info *gi);
void    get_game_info_from_server(struct socket_info *si, struct game_info *gi);
void    send_game_info_to_server(struct socket_info *si, struct game_info *gi);
void    update_paddle(struct game_info *gi);

int main(int argc, char **argv)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);


    struct game_info gi;

    gi.upper_paddle_x = (SCREEN_WIDTH - PADDLE_WIDTH) / 2;
    gi.lower_paddle_x = (SCREEN_WIDTH - PADDLE_WIDTH) / 2;
    gi.ball_pos.x = SCREEN_WIDTH / 2 - BALL_RADIUS;
    gi.ball_pos.y = SCREEN_HEIGHT / 2 - BALL_RADIUS;

    getchar();

    struct socket_info si = {};
    si.server_addr = make_address(argv[1], argv[2]);
    create_client(&si);

    while (!WindowShouldClose())
    {
        //Update
        update_paddle(&gi);
        send_game_info_to_server(&si, &gi);
        get_game_info_from_server(&si, &gi);

        //Draw
        BeginDrawing();

        ClearBackground(BLACK);

        draw_ball_and_paddles(&gi);

        EndDrawing();
    }

    release(&si);

    CloseWindow();

    return 0;
}

void    draw_ball_and_paddles(struct game_info *gi)
{
    DrawCircle(gi->ball_pos.x, gi->ball_pos.y, BALL_RADIUS, RED);
    DrawRectangle(gi->lower_paddle_x, SCREEN_HEIGHT - PADDLE_HEIGHT, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
    DrawRectangle(gi->upper_paddle_x, 0, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
}

void    get_game_info_from_server(struct socket_info *si, struct game_info *gi)
{
    float buf[3];
    ssize_t ret = recv(si->client_socket, buf, sizeof(buf), 0);
    if (ret < 0) {
        perror("recv()");
        exit(EXIT_FAILURE);
    }
    gi->ball_pos.x = buf[0];
    gi->ball_pos.y = buf[1];
    gi->upper_paddle_x = buf[2];
}

void    send_game_info_to_server(struct socket_info *si, struct game_info *gi)
{
    if (send(si->client_socket, &gi->lower_paddle_x, sizeof(float ), 0) < 0) {
        perror("sendto()");
        exit(EXIT_FAILURE);
    }
}

void    update_paddle(struct game_info *gi)
{
    float new_pos = GetMouseX() - PADDLE_WIDTH / 2;
    if(new_pos < 0)
    {
        new_pos = 0;
    } else if(new_pos > (SCREEN_WIDTH - PADDLE_WIDTH))
    {
        new_pos = SCREEN_WIDTH - PADDLE_WIDTH;
    }
    gi->lower_paddle_x = new_pos;
}
