//
// Created by I3artek on 03/04/2023.
//

#include <stdlib.h>
#include "game.h"
#include "nettools.h"

int main()
{
    //fprintf(stderr, "%lld", getpid());

    struct game_info gameInfo;
    gameInfo.ball_pos.x = SCREEN_WIDTH / 2 - BALL_RADIUS;
    gameInfo.ball_pos.y = SCREEN_WIDTH / 2 - BALL_RADIUS;

    struct socket_info player_one;
    struct socket_info player_two;

    create_server(&player_one);
    accept_client(&player_one);

    while (1)
    {
        send_game_info_to_client(&player_one, &gameInfo);
        get_game_info_from_client(&player_one, &gameInfo);
    }


    return EXIT_SUCCESS;
}
