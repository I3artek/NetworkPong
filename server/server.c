//
// Created by I3artek on 03/04/2023.
//

#include <stdlib.h>
#include "../sources/game.h"
#include "../sources/nettools.h"

#define VELOCITY 1

struct  game_info
{
    float    upper_paddle_x;
    float    lower_paddle_x;
    float    ball_x;
    float    ball_y;
};

void    move_ball(struct game_info *gi, float *vx, float *vy);
void    send_game_info_to_client(struct socket_info *si, float, float, float pos);
void    get_game_info_from_client(struct socket_info *si, float *pos);

int main()
{

    struct game_info gi;
    gi.ball_x = SCREEN_WIDTH / 2 - BALL_RADIUS;
    gi.ball_y = SCREEN_WIDTH / 2 - BALL_RADIUS;

    struct socket_info player_one;
    struct socket_info player_two;

    player_one.server_addr = make_address("127.0.0.1", "12345");
    create_server(&player_one);
    accept_client(&player_one);

    player_two.server_addr = make_address("127.0.0.1", "12346");
    create_server(&player_two);
    accept_client(&player_two);

    float vx = VELOCITY;
    float vy = VELOCITY;

    while (1)
    {
        move_ball(&gi, &vx, &vy);
        send_game_info_to_client(&player_one, gi.ball_x, gi.ball_y, gi.upper_paddle_x);
        send_game_info_to_client(&player_two, gi.ball_x, SCREEN_HEIGHT - gi.ball_y, gi.lower_paddle_x);
        get_game_info_from_client(&player_one, &gi.lower_paddle_x);
        get_game_info_from_client(&player_two, &gi.upper_paddle_x);
    }

    release(&player_one);
    release(&player_two);

    return EXIT_SUCCESS;
}

void    move_ball(struct game_info *gi, float *vx, float *vy)
{
    gi->ball_x += *vx;
    gi->ball_y += *vy;
    
    if(gi->ball_x > gi->upper_paddle_x &&
    gi->ball_x < gi->upper_paddle_x + PADDLE_WIDTH &&
    gi->ball_y < PADDLE_HEIGHT + BALL_RADIUS)
    {
        //hits the upper paddle
        *vy *= -1;
    } else if(gi->ball_x > gi->lower_paddle_x &&
              gi->ball_x < gi->lower_paddle_x + PADDLE_WIDTH &&
              gi->ball_y > SCREEN_HEIGHT - PADDLE_HEIGHT - BALL_RADIUS)
    {
        //hits the lower paddle
        *vy *= -1;
    } else if(gi->ball_y < BALL_RADIUS)
    {
        //hits upper border
        *vy *= -1;
    } else if(gi->ball_y > (SCREEN_HEIGHT - BALL_RADIUS))
    {
        //hits lower border
        *vy *= -1;
    } else if(gi->ball_x < BALL_RADIUS)
    {
        //hits left border
        *vx *= -1;
    } else if(gi->ball_x > (SCREEN_WIDTH - BALL_RADIUS))
    {
        //hits right border
        *vx *= -1;
    }
}

void    send_game_info_to_client(struct socket_info *si, float ball_x, float ball_y, float paddle_pos)
{
    float buf[3];
    buf[0] = ball_x;
    buf[1] = ball_y;
    buf[2] = paddle_pos;
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
