//
// Created by I3artek on 03/04/2023.
//

#ifndef MY_RAYLIB_GAME_GAME_H
#define MY_RAYLIB_GAME_GAME_H

#include "raylib.h"

#include "paddle.h"
#include "ball.h"

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (450)

struct  game_info
{
    int     upper_paddle_X;
    int     lower_paddle_X;
    Vector2 ball_pos;
};

void    draw_ball_and_paddles(struct game_info *gi)
{
    DrawCircle(gi->ball_pos.x, gi->ball_pos.y, BALL_RADIUS, RED);
    DrawRectangle(gi->lower_paddle_X, SCREEN_HEIGHT - PADDLE_HEIGHT, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
    DrawRectangle(gi->upper_paddle_X, 0, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
}

#endif //MY_RAYLIB_GAME_GAME_H
