#ifndef TEST
#include "raylib.h"
#include "Sim.h"

void draw_cell(int i)
{
    div_t result = div(i, settings::board_width);
    DrawRectangle( result.rem*settings::square_size,
                   result.quot*settings::square_size-1,
                   settings::square_size-1,
                   settings::square_size, WHITE);
}

void draw_background()
{
    ClearBackground(WHITE);
    for (int i = 0; i < settings::cell_count; i++) {
        div_t result = div(i, settings::board_width);
        DrawRectangle( result.rem*settings::square_size,
                       result.quot*settings::square_size,
                       settings::square_size-1,
                       settings::square_size-1, BLACK);
    }
}

int main()
{
    InitWindow( settings::board_width*settings::square_size,
                settings::board_width*settings::square_size,
                "Game of Life");

    SetTargetFPS(2);

    Sim sim;

    sim.generate_seed();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        draw_background();
        state_t current_state = sim.get_state();
        for (int i = 0; i < settings::cell_count; i++)
            if (live == current_state[i])
                draw_cell(i);
        sim.evolve();
        EndDrawing();
    }
    CloseWindow();
}
#endif
