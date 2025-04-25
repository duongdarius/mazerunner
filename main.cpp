#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "maze.h"
#include "player.h"
#include "enemy.h"

struct Explosion {
    int x, y;
    int frame;
    float frameTime;
    float accumulator;
    bool finished;
    bool forGameOver;
};

std::vector<Explosion> explosions;
std::vector<Enemy> enemies;

const int EXPLOSION_FRAME_COUNT = 8;
const int EXPLOSION_FRAME_WIDTH = 64;
const int EXPLOSION_FRAME_HEIGHT = 64;

bool CheckCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return !(a.x + a.w <= b.x || b.x + b.w <= a.x || a.y + a.h <= b.y || b.y + b.h <= a.y);
}

bool CheckWallCollision(const Maze& maze, const SDL_Rect& rect) {
    int left = rect.x, right = rect.x + rect.w - 1;
    int top = rect.y, bottom = rect.y + rect.h - 1;
    return (
        maze.IsWall(top / Maze::TILE_SIZE, left / Maze::TILE_SIZE) ||
        maze.IsWall(top / Maze::TILE_SIZE, right / Maze::TILE_SIZE) ||
        maze.IsWall(bottom / Maze::TILE_SIZE, left / Maze::TILE_SIZE) ||
        maze.IsWall(bottom / Maze::TILE_SIZE, right / Maze::TILE_SIZE)
    );
}

bool CheckCollisionInclusive(const SDL_Rect& a, const SDL_Rect& b) {
    return !(a.x + a.w < b.x || b.x + b.w < a.x || a.y + a.h < b.y || b.y + b.h < a.y);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    Mix_Chunk* shootSound = Mix_LoadWAV("C:\\Projects\\SDL\\MazeRunner\\bin\\Debug\\assets\\shoot.wav");
    Mix_Chunk* hitSound = Mix_LoadWAV("C:\\Projects\\SDL\\MazeRunner\\bin\\Debug\\assets\\hit.wav");

    int screenWidth = Maze::MAP_COLS * Maze::TILE_SIZE;
    int screenHeight = Maze::MAP_ROWS * Maze::TILE_SIZE;
    SDL_Window* window = SDL_CreateWindow("Maze + Player + Enemy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Maze maze;
    maze.Init(renderer);

    SDL_Surface* surf = IMG_Load("C:\\Projects\\SDL\\MazeRunner\\bin\\Debug\\assets\\player.png");
    SDL_Texture* playerTex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);

    Player player;
    player.SetShootSound(shootSound);
    player.SetWindowSize(screenWidth, screenHeight);
    player.Init(renderer);

    surf = IMG_Load("C:\\Projects\\SDL\\MazeRunner\\bin\\Debug\\assets\\enemy.png");
    SDL_Texture* enemyTex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);

    for (auto& pos : maze.GetEnemySpawnPositions()) {
        enemies.emplace_back();
        enemies.back().Init(pos.x, pos.y, enemyTex);
    }

    surf = IMG_Load("C:\\Projects\\SDL\\MazeRunner\\bin\\Debug\\assets\\explosion.png");
    SDL_Texture* explosionTex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);

    bool gameOverPending = false, playerExploded = false;
    bool running = true;
    bool isPaused = false;
    SDL_Event e;
    Uint32 lastTicks = SDL_GetTicks();

    while (running) {
        Uint32 currentTicks = SDL_GetTicks();
        float deltaTime = (currentTicks - lastTicks) / 1000.0f;
        lastTicks = currentTicks;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN && !e.key.repeat) {
                if (e.key.keysym.sym == SDLK_p) {
                    isPaused = !isPaused;
                }
            }
            if (!isPaused) player.HandleEvent(e);
        }

        if (!isPaused) {
            SDL_Rect oldRect = player.GetRect();
            int oldX = oldRect.x, oldY = oldRect.y;
            player.Update(deltaTime);
            if (CheckWallCollision(maze, player.GetCollisionRect()))
                player.SetPosition(oldX, oldY);

            for (auto& e : enemies) e.Update(deltaTime, maze);

            auto& bullets = player.GetBulletsRef();
            for (auto& e : enemies) {
                if (!e.IsAlive()) continue;
                for (auto& b : bullets) {
                    if (CheckWallCollision(maze, b.rect)) {
                        b.active = false;
                        continue;
                    }

                    if (b.active && CheckCollision(b.rect, e.GetCollisionRect())) {
                        e.TakeDamage(1);
                        b.active = false;
                        explosions.push_back({ e.GetRect().x, e.GetRect().y, 0, 0.1f, 0.0f, false, false });
                        Mix_PlayChannel(-1, hitSound, 0);
                        break;
                    }
                }

                if (!playerExploded && e.IsAlive() && CheckCollision(player.GetCollisionRect(), e.GetCollisionRect())) {
                    explosions.push_back({ player.GetRect().x, player.GetRect().y, 0, 0.1f, 0.0f, false, true });
                    Mix_PlayChannel(-1, hitSound, 0);
                    gameOverPending = true;
                    playerExploded = true;
                }
            }

            for (auto it = enemies.begin(); it != enemies.end(); ) {
                if (!it->IsAlive()) it = enemies.erase(it);
                else ++it;
            }

            for (size_t i = 0; i < explosions.size(); ) {
                Explosion& exp = explosions[i];
                exp.accumulator += deltaTime;
                if (exp.accumulator >= exp.frameTime) {
                    exp.frame++;
                    exp.accumulator -= exp.frameTime;
                    if (exp.frame >= EXPLOSION_FRAME_COUNT) exp.finished = true;
                }
                if (exp.finished) explosions.erase(explosions.begin() + i);
                else i++;
            }

            if (gameOverPending) {
                bool active = false;
                for (const auto& exp : explosions) if (exp.forGameOver) active = true;
                if (!active) {
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game Over", "Bạn đã thua!", window);
                    running = false;
                }
            }

            // Xử lý nhặt vật phẩm
            SDL_Rect pRect = player.GetCollisionRect();
            int row = pRect.y / Maze::TILE_SIZE;
            int col = pRect.x / Maze::TILE_SIZE;
            int tile = maze.GetTile(row, col);

            if (tile == 4) {
                player.doubleBullet = true;
                maze.SetTile(row, col, 0);
            }
            if (tile == 5) {
                player.speedBoost = true;
                maze.SetTile(row, col, 0);
            }

            // Kiểm tra về đích
            for (int rr = 0; rr < Maze::MAP_ROWS; rr++) {
                for (int cc = 0; cc < Maze::MAP_COLS; cc++) {
                    if (maze.GetTile(rr, cc) == 2) {
                        SDL_Rect tileRect = { cc * Maze::TILE_SIZE, rr * Maze::TILE_SIZE, Maze::TILE_SIZE, Maze::TILE_SIZE };
                        if (CheckCollisionInclusive(pRect, tileRect)) {
                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "You Win!", "Chúc mừng, bạn đã đến kho báu!", window);
                            running = false;
                            break;
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        maze.Render(renderer);
        if (!playerExploded) player.Render(renderer);
        for (auto& e : enemies) e.Render(renderer);
        for (auto& exp : explosions) {
            SDL_Rect src = { exp.frame * EXPLOSION_FRAME_WIDTH, 0, EXPLOSION_FRAME_WIDTH, EXPLOSION_FRAME_HEIGHT };
            SDL_Rect dst = { exp.x, exp.y, EXPLOSION_FRAME_WIDTH, EXPLOSION_FRAME_HEIGHT };
            SDL_RenderCopy(renderer, explosionTex, &src, &dst);
        }

        if (isPaused) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
            SDL_Rect pauseBox = { screenWidth / 2 - 60, screenHeight / 2 - 20, 120, 40 };
            SDL_RenderFillRect(renderer, &pauseBox);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(enemyTex);
    SDL_DestroyTexture(playerTex);
    SDL_DestroyTexture(explosionTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
