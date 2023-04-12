//
// Created by I3artek on 03/04/2023.
//

#ifndef MY_RAYLIB_GAME_GAME_H
#define MY_RAYLIB_GAME_GAME_H

#define SCREEN_WIDTH (600)
#define SCREEN_HEIGHT (400)

#define BALL_RADIUS 20

#define PADDLE_WIDTH 200
#define PADDLE_HEIGHT 40

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT "12345"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

struct  client_socket
{
    struct  sockaddr    client_addr;
    int     sock;
    socklen_t client_addrlen;
};

struct  sockaddr_in make_address(const char *, const char *);
struct sockaddr_in make_address(const char *address, const char *port) {
    int ret;
    struct sockaddr_in addr;
    struct addrinfo *result;
    struct addrinfo hints = {};
    hints.ai_family = AF_INET;
    if ((ret = getaddrinfo(address, port, &hints, &result))) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        exit(EXIT_FAILURE);
    }
    addr = *(struct sockaddr_in *) (result->ai_addr);
    freeaddrinfo(result);
    return addr;
}

#endif //MY_RAYLIB_GAME_GAME_H
