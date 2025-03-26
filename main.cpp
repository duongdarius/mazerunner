#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Maze.h"
#include "Player.h"
#include "Enemy.h"

int main() {
    Maze maze;
    Player player;
    Enemy enemy;

    SDL_Window* window(SDL_CreateWindow(WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE), "Maze Runner");

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_Event::Closed)
                window.close();
            if (event.type == SDL_Event::KeyPressed)
                player.move(event.key.code, maze);
        }

        enemy.move(maze);

        SDL_RenderClear(renderer);
        maze.draw(window);
        player.draw(window);
        enemy.draw(window);
        SDL_RenderPresent(renderer);
    }

    return 0;
}
