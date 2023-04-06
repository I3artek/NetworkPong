#include "raylib.h"

#include "game.h"
#include "nettools.h"

#define WINDOW_TITLE "Window title"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);


    struct game_info gi;

    gi.upper_paddle_X = (SCREEN_WIDTH - PADDLE_WIDTH) / 2;
    gi.lower_paddle_X = (SCREEN_WIDTH - PADDLE_WIDTH) / 2;
    gi.ball_pos.x = SCREEN_WIDTH / 2 - BALL_RADIUS;
    gi.ball_pos.y = SCREEN_HEIGHT / 2 - BALL_RADIUS;

    struct socket_info si = {};
    //create_client(&si);

    while (!WindowShouldClose())
    {
        //Update

        send_game_info(&si, &gi);
        get_game_info(&si, &gi);

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
