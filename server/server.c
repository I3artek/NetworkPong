//
// Created by I3artek on 03/04/2023.
//

#include <stdlib.h>
#include "game.h"

#define VELOCITY 1

struct  game_info
{
    float    upper_paddle_x;
    float    lower_paddle_x;
    float    ball_x;
    float    ball_y;
};

struct server_socket
{
    struct  sockaddr_in server_addr;
    int     sock;
};

void    move_ball(struct game_info *, float *, float *);
void    send_game_info_to_client(struct client_socket *, float, float, float);
void    get_game_info_from_client(struct client_socket *, float *);
void    create_server(struct server_socket *);
void    accept_client(struct client_socket *, struct server_socket *);

int main()
{

    struct game_info gi;
    gi.ball_x = SCREEN_WIDTH / 2 - BALL_RADIUS;
    gi.ball_y = SCREEN_WIDTH / 2 - BALL_RADIUS;

    struct server_socket serverSocket;
    struct client_socket playerOne;
    struct client_socket playerTwo;

    create_server(&serverSocket);
    accept_client(&playerOne, &serverSocket);
    accept_client(&playerTwo, &serverSocket);

    float vx = VELOCITY;
    float vy = VELOCITY;

    while (1)
    {
        move_ball(&gi, &vx, &vy);
        send_game_info_to_client(&playerOne, gi.ball_x, gi.ball_y, gi.upper_paddle_x);
        send_game_info_to_client(&playerTwo, gi.ball_x, SCREEN_HEIGHT - gi.ball_y, gi.lower_paddle_x);
        get_game_info_from_client(&playerOne, &gi.lower_paddle_x);
        get_game_info_from_client(&playerTwo, &gi.upper_paddle_x);
    }

    close(serverSocket.sock);
    close(playerOne.sock);
    close(playerTwo.sock);

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

void    send_game_info_to_client(struct client_socket *cs, float ball_x, float ball_y, float paddle_pos)
{
    float buf[3];
    buf[0] = ball_x;
    buf[1] = ball_y;
    buf[2] = paddle_pos;
    if (send(cs->sock, buf, sizeof(buf), 0) < 0) {
        perror("sendto()");
        exit(EXIT_FAILURE);
    }
}

void    get_game_info_from_client(struct client_socket *cs, float *pos)
{
    ssize_t ret = recv(cs->sock, pos, sizeof(float), 0);
    if (ret < 0) {
        perror("recv()");
        exit(EXIT_FAILURE);
    }
}

void    create_server(struct server_socket *ss)
{
    ss->server_addr = make_address(SERVER_ADDRESS, SERVER_PORT);

    ss->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (ss->sock < 0)
    {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    if (bind(ss->sock,
             (struct sockaddr *) &ss->server_addr,
             sizeof(ss->server_addr)) < 0)
    {
        perror("bind()");
        exit(EXIT_FAILURE);
    }

    if (listen(ss->sock, 2) < 0)
    {
        perror("listen()");
        exit(EXIT_FAILURE);
    }
}

void    accept_client(struct client_socket *cs, struct server_socket *ss)
{
    cs->client_addrlen = sizeof(cs->client_addr);
    cs->sock = accept(ss->sock, &cs->client_addr, &cs->client_addrlen);
    if (cs->sock < 0)
    {
        perror("accept()");
        exit(EXIT_FAILURE);
    }
}
