#include "Maze.h"
#include <stack>
#include <ctime>
#include <cstdlib>
#include <random>

Maze::Maze() {
    grid.resize(HEIGHT, std::vector<char>(WIDTH, WALL));
    exitPos = {WIDTH - 2, HEIGHT - 2};
    generate();
}

void Maze::generate() {
    srand(time(0));
    std::stack<SDL_Point> stk;
    SDL_Point start(1, 1);
    grid[start.y][start.x] = PATH;
    stk.push(start);

    std::random_device rd;
    std::mt19937 g(rd());
    const int dx[] = {1, -1, 0, 0};
    const int dy[] = {0, 0, 1, -1};

    while (!stk.empty()) {
        SDL_Point current = stk.top();
        std::vector<int> directions = {0, 1, 2, 3};
        std::shuffle(directions.begin(), directions.end(), g);
        bool moved = false;

        for (int i : directions) {
            int nx = current.x + dx[i] * 2;
            int ny = current.y + dy[i] * 2;
            if (nx > 0 && ny > 0 && nx < WIDTH - 1 && ny < HEIGHT - 1 && grid[ny][nx] == WALL) {
                grid[ny][nx] = PATH;
                grid[current.y + dy[i]][current.x + dx[i]] = PATH;
                stk.push({nx, ny});
                moved = true;
                break;
            }
        }
        if (!moved) stk.pop();
    }
    grid[exitPos.y][exitPos.x] = EXIT;
}

bool Maze::isValidMove(int x, int y) {
    return (x > 0 && y > 0 && x < WIDTH - 1 && y < HEIGHT - 1 && grid[y][x] != WALL);
}

void Maze::draw(SDL_Window* &window) {
    SDL_Rect tile(SDL_FPoint(TILE_SIZE, TILE_SIZE));
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            tile.setPosition(x * TILE_SIZE, y * TILE_SIZE);
            if (grid[y][x] == WALL) tile.setFillColor(SDL_Color::Blue);
            else if (grid[y][x] == EXIT) tile.setFillColor(SDL_Color::Red);
            else tile.setFillColor(SDL_Color::White);
            SDL_RenderFillRect(renderer, tile);
        }
    }
}
