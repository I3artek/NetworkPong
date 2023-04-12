#include "raylib.h"

#include "game.h"

#define WINDOW_TITLE "Window title"
#define CONNECT_BTN_WIDTH 400
#define CONNECT_BTN_HEIGHT 100

struct  game_info
{
    float   upper_paddle_x;
    float   lower_paddle_x;
    Vector2 ball_pos;
};

void    draw_ball_and_paddles(struct game_info *);
void    get_game_info_from_server(struct client_socket *, struct game_info *);
void    send_game_info_to_server(struct client_socket *, struct game_info *);
void    update_paddle(struct game_info *);
void    create_client(struct client_socket *);

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);


    struct game_info gi;

    gi.upper_paddle_x = (SCREEN_WIDTH - PADDLE_WIDTH) / 2;
    gi.lower_paddle_x = (SCREEN_WIDTH - PADDLE_WIDTH) / 2;
    gi.ball_pos.x = SCREEN_WIDTH / 2 - BALL_RADIUS;
    gi.ball_pos.y = SCREEN_HEIGHT / 2 - BALL_RADIUS;

    while (!WindowShouldClose() && !IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        BeginDrawing();

        ClearBackground(BLACK);

        draw_ball_and_paddles(&gi);

        EndDrawing();
    }

    struct client_socket cs = {};
    create_client(&cs);

    while (!WindowShouldClose())
    {
        //Update
        update_paddle(&gi);
        send_game_info_to_server(&cs, &gi);
        get_game_info_from_server(&cs, &gi);

        //Draw
        BeginDrawing();

        ClearBackground(BLACK);

        draw_ball_and_paddles(&gi);

        EndDrawing();
    }

    close(cs.sock);

    CloseWindow();

    return 0;
}

void    draw_ball_and_paddles(struct game_info *gi)
{
    DrawCircle(gi->ball_pos.x, gi->ball_pos.y, BALL_RADIUS, RED);
    DrawRectangle(gi->lower_paddle_x, SCREEN_HEIGHT - PADDLE_HEIGHT, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
    DrawRectangle(gi->upper_paddle_x, 0, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
}

void    get_game_info_from_server(struct client_socket *cs, struct game_info *gi)
{
    float buf[3];
    ssize_t ret = recv(cs->sock, buf, sizeof(buf), 0);
    if (ret < 0) {
        perror("recv()");
        exit(EXIT_FAILURE);
    }
    gi->ball_pos.x = buf[0];
    gi->ball_pos.y = buf[1];
    gi->upper_paddle_x = buf[2];
}

void    send_game_info_to_server(struct client_socket *cs, struct game_info *gi)
{
    if (send(cs->sock, &gi->lower_paddle_x, sizeof(float ), 0) < 0) {
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

void    create_client(struct client_socket *cs)
{
    struct sockaddr_in server_addr = make_address(SERVER_ADDRESS, SERVER_PORT);

    cs->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (cs->sock < 0) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    if (connect(cs->sock,
                (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0)
    {
        perror("connect()");
        exit(EXIT_FAILURE);
    }
}
