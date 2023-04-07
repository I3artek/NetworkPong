//
// Created by I3artek on 03/04/2023.
//

#include <stdlib.h>
#include "../sources/game.h"
#include "../sources/nettools.h"

struct  game_info
{
    float    upper_paddle_x;
    float    lower_paddle_x;
    float    ball_x;
    float    ball_y;
};

void    move_ball(struct game_info *gi, float vx, float vy);
void    send_game_info_to_client(struct socket_info *si, struct game_info *gi, float pos);
void    get_game_info_from_client(struct socket_info *si, float *pos);

int main()
{
    //fprintf(stderr, "%lld", getpid());

    struct game_info gameInfo;
    gameInfo.ball_x = SCREEN_WIDTH / 2 - BALL_RADIUS;
    gameInfo.ball_y = SCREEN_WIDTH / 2 - BALL_RADIUS;

    struct socket_info player_one;
    struct socket_info player_two;

    player_one.server_addr = make_address("127.0.0.1", "12345");
    create_server(&player_one);
    accept_client(&player_one);

    player_two.server_addr = make_address("127.0.0.1", "12346");
    create_server(&player_two);
    accept_client(&player_two);

    float vx = 1;
    float vy = 1;

    while (1)
    {
        move_ball(&gameInfo, vx, vy);
        send_game_info_to_client(&player_one, &gameInfo, gameInfo.upper_paddle_x);
        send_game_info_to_client(&player_two, &gameInfo, gameInfo.lower_paddle_x);
        get_game_info_from_client(&player_one, &gameInfo.lower_paddle_x);
        get_game_info_from_client(&player_two, &gameInfo.upper_paddle_x);
    }

    release(&player_one);
    release(&player_two);

    return EXIT_SUCCESS;
}

void    move_ball(struct game_info *gi, float vx, float vy)
{
    gi->ball_x += vx;
    gi->ball_y += vy;
}

void    send_game_info_to_client(struct socket_info *si, struct game_info *gi, float pos)
{
    float buf[3];
    buf[0] = gi->ball_x;
    buf[1] = gi->ball_y;
    buf[2] = pos;
    if (send(si->client_socket, buf, sizeof(buf), 0) < 0) {
        perror("sendto()");
        exit(EXIT_FAILURE);
    }
}

void    get_game_info_from_client(struct socket_info *si, float *pos)
{
    ssize_t ret = recv(si->client_socket, pos, sizeof(float), 0);
    if (ret < 0) {
        perror("recv()");
        exit(EXIT_FAILURE);
    }
}
