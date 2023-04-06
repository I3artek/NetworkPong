//
// Created by I3artek on 03/04/2023.
//

#ifndef MY_RAYLIB_GAME_NETTOOLS_H
#define MY_RAYLIB_GAME_NETTOOLS_H

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

struct  socket_info
{
    struct  sockaddr_in server_addr;
    int     server_socket;
    struct  sockaddr    client_addr;
    int     client_socket;
    socklen_t client_addrlen;
};

struct  sockaddr_in make_address(const char *, const char *);

void    create_server(struct socket_info *);
void    create_client(struct socket_info *);
void    set_address(struct socket_info *);
void    accept_client(struct socket_info *);
void    release(struct socket_info *);

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

void    create_server(struct socket_info *si)
{
    set_address(si);

    si->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (si->server_socket < 0)
    {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    if (bind(si->server_socket,
             (struct sockaddr *) &si->server_addr,
             sizeof(si->server_addr)) < 0)
    {
        perror("bind()");
        exit(EXIT_FAILURE);
    }

    if (listen(si->server_socket, 3) < 0)
    {
        perror("listen()");
        exit(EXIT_FAILURE);
    }
}

void    create_client(struct socket_info *si)
{
    set_address(si);

    si->client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (si->client_socket < 0) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    if (connect(si->client_socket,
                (struct sockaddr *) &si->server_addr,
                sizeof(si->server_addr)) < 0)
    {
        perror("connect()");
        exit(EXIT_FAILURE);
    }
}

void    set_address(struct socket_info *si)
{
//    char host[16];
//    char port[6];
//
//    printf("Type host address >");
//    fflush(stdout);
//    scanf("%15s", host);
//    fflush(stdin);
//    printf("Type port >");
//    fflush(stdout);
//    scanf("%5s", port);

    si->server_addr = make_address("127.0.0.1", "12345");
}

void    accept_client(struct socket_info *si)
{
    si->client_addrlen = sizeof(si->client_addr);
    si->client_socket = accept(si->server_socket,
                               &si->client_addr, &si->client_addrlen);
    if (si->client_socket < 0)
    {
        perror("accept()");
        exit(EXIT_FAILURE);
    }
}

void    release(struct socket_info *si)
{
    if(si->client_socket != 0)
        close(si->client_socket);
    if(si->server_socket != 0)
        close(si->server_socket);
}

void    get_game_info_from_server(struct socket_info *si, struct game_info *gi)
{

}

void    send_game_info_to_server(struct socket_info *si, struct game_info *gi)
{

}

void    get_game_info_from_client(struct socket_info *si, struct game_info *gi)
{

}

void    send_game_info_to_client(struct socket_info *si, struct game_info *gi)
{

}


#endif //MY_RAYLIB_GAME_NETTOOLS_H
