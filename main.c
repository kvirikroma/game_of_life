#include <stdio.h>
#include <stdlib.h>

#include "include/bit_array2d.h"
#include "include/life_runner.h"
#include "include/utils.h"
#include "include/life_drawer.h"


/*int main(int argc, char** argv)
{
    life_runner game;
    life_runner_init(&game, 32, 18);
    game.neighbors_that_matter = 0b01011010;  // von Neumann neighborhood

    bit_array2d_set_bit(game.field, 2, 4, 1);
    bit_array2d_set_bit(game.field, 3, 4, 1);
    bit_array2d_set_bit(game.field, 4, 4, 1);

    bit_array2d_set_bit(game.field, 0, 0, 1);
    bit_array2d_set_bit(game.field, 1, 0, 1);
    bit_array2d_set_bit(game.field, 0, 1, 1);

    for (uint32_t i = 0; i < 4; i++)
    {
        print_field(game.field);
        printf("\n");
        life_runner_make_step(&game);
    }

    life_runner_delete(&game);
    return 0;
}*/

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        exit(1);
    }

    SDL_Window* window = SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    
    if (!window) {
        exit(1);
    }

    SDL_Surface* screen_surface = SDL_GetWindowSurface(window);
    SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0, 0, 0));
    SDL_UpdateWindowSurface(window);

    SDL_Delay(8000);
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
