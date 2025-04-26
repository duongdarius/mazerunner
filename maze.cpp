#include "maze.h"
#include <SDL2/SDL_image.h>
#include <iostream>

Maze::Maze() {
    wallTex = nullptr;
    floorTex = nullptr;
    goalTex = nullptr;
    ammoTex = nullptr;
    speedTex = nullptr;
}

Maze::~Maze() {
    if (wallTex) SDL_DestroyTexture(wallTex);
    if (floorTex) SDL_DestroyTexture(floorTex);
    if (goalTex) SDL_DestroyTexture(goalTex);
    if (ammoTex) SDL_DestroyTexture(ammoTex);
    if (speedTex) SDL_DestroyTexture(speedTex);
}

bool Maze::Init(SDL_Renderer* renderer, int level) {
    floorTex = LoadTexture(renderer, "C:\\Projects\\SDL\\MazeRunner\\bin\\Debug\\assets\\floor.png");
    wallTex  = LoadTexture(renderer, "C:\\Projects\\SDL\\MazeRunner\\bin\\Debug\\assets\\wall.png");
    goalTex  = LoadTexture(renderer, "C:\\Projects\\SDL\\MazeRunner\\bin\\Debug\\assets\\goal.png");
    ammoTex  = LoadTexture(renderer, "C:\\Projects\\SDL\\MazeRunner\\bin\\Debug\\assets\\ammo.png");
    speedTex = LoadTexture(renderer, "C:\\Projects\\SDL\\MazeRunner\\bin\\Debug\\assets\\speed.png");

    if (level == 1) {
        const int defaultMap1[MAP_ROWS][MAP_COLS] = {
            {0,1,1,1,1,0,0,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,1,1,1,1,0,1,1,0,1,1},
            {1,1,1,0,1,0,0,0,0,0,0,3,0,1,1},
            {1,3,0,0,1,0,1,1,0,1,1,0,0,0,1},
            {1,4,1,0,1,0,0,0,1,0,1,0,1,1,1},
            {1,0,1,0,1,0,1,1,1,0,0,0,1,0,1},
            {1,0,1,0,1,0,1,5,1,0,1,1,1,0,1},
            {1,0,1,0,1,0,1,0,1,3,0,0,1,0,1},
            {1,0,0,0,3,0,0,0,1,1,1,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,1,0,1,0,2}
        };

        for (int r = 0; r < MAP_ROWS; ++r)
            for (int c = 0; c < MAP_COLS; ++c)
                map[r][c] = defaultMap1[r][c];
    }
    else if (level == 2) {
        const int defaultMap2[MAP_ROWS][MAP_COLS] = {
            {0,0,0,0,0,0,4,1,0,0,0,1,1,0,0},
            {1,1,0,3,0,1,1,1,1,0,1,1,0,0,1},
            {1,0,1,1,0,0,0,0,1,0,0,3,0,1,1},
            {1,0,1,1,1,0,1,0,3,0,1,0,0,0,1},
            {1,5,0,0,1,0,0,1,1,0,0,0,1,1,1},
            {1,1,1,0,1,0,1,0,1,1,1,0,1,0,1},
            {1,0,0,0,1,0,1,0,1,0,3,0,1,0,1},
            {1,0,1,1,1,0,1,0,1,0,0,0,1,0,1},
            {1,0,0,3,0,0,0,0,1,1,1,0,3,0,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,0,2}
        };

        for (int r = 0; r < MAP_ROWS; ++r)
            for (int c = 0; c < MAP_COLS; ++c)
                map[r][c] = defaultMap2[r][c];
    }

    return floorTex && wallTex && goalTex && ammoTex && speedTex;
}

void Maze::Render(SDL_Renderer* renderer) {
    SDL_Rect bgRect = { 0, 0, MAP_COLS * TILE_SIZE, MAP_ROWS * TILE_SIZE };
    SDL_RenderCopy(renderer, floorTex, nullptr, &bgRect);

    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            int tile = map[r][c];
            SDL_Rect dst = { c * TILE_SIZE, r * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            if (tile == 1) {
                SDL_RenderCopy(renderer, wallTex, nullptr, &dst);
            } else if (tile == 2) {
                SDL_RenderCopy(renderer, goalTex, nullptr, &dst);
            } else if (tile == 4) {
                SDL_Rect dstSmall = { c * TILE_SIZE + TILE_SIZE / 4, r * TILE_SIZE + TILE_SIZE / 4, TILE_SIZE / 2, TILE_SIZE / 2 };
                SDL_RenderCopy(renderer, ammoTex, nullptr, &dstSmall);
            } else if (tile == 5) {
                SDL_Rect dstSmall = { c * TILE_SIZE + TILE_SIZE / 4, r * TILE_SIZE + TILE_SIZE / 4, TILE_SIZE / 2, TILE_SIZE / 2 };
                SDL_RenderCopy(renderer, speedTex, nullptr, &dstSmall);
            }
        }
    }
}

bool Maze::IsWall(int row, int col) const {
    if (row < 0 || row >= MAP_ROWS || col < 0 || col >= MAP_COLS)
        return true;
    return map[row][col] == 1;
}

int Maze::GetTile(int row, int col) const {
    if (row < 0 || row >= MAP_ROWS || col < 0 || col >= MAP_COLS)
        return 1;
    return map[row][col];
}

void Maze::SetTile(int row, int col, int value) {
    if (row >= 0 && row < MAP_ROWS && col >= 0 && col < MAP_COLS) {
        map[row][col] = value;
    }
}

std::vector<SDL_Point> Maze::GetEnemySpawnPositions() const {
    std::vector<SDL_Point> result;
    for (int r = 0; r < MAP_ROWS; ++r) {
        for (int c = 0; c < MAP_COLS; ++c) {
            if (map[r][c] == 3) {
                result.push_back({ c * TILE_SIZE, r * TILE_SIZE });
            }
        }
    }
    return result;
}

SDL_Texture* Maze::LoadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) {
        std::cout << "IMG_Load Error: " << IMG_GetError() << " Path: " << path << std::endl;
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    return tex;
}
