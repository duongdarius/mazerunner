#ifndef MAZE_H
#define MAZE_H

#include <SDL2/SDL.h>
#include <vector>

class Maze {
public:
    Maze();
    ~Maze();

    bool Init(SDL_Renderer* renderer, int level);
    void Render(SDL_Renderer* renderer);

    bool IsWall(int row, int col) const;
    int GetTile(int row, int col) const;
    void SetTile(int row, int col, int value);

    std::vector<SDL_Point> GetEnemySpawnPositions() const;

    static const int TILE_SIZE = 64;
    static const int MAP_ROWS = 10;
    static const int MAP_COLS = 15;

private:
    int map[MAP_ROWS][MAP_COLS];
    SDL_Texture* floorTex;
    SDL_Texture* wallTex;
    SDL_Texture* goalTex;
    SDL_Texture* ammoTex;
    SDL_Texture* speedTex;

    SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* path);
};

#endif
