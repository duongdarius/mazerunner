#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>

enum Direction { LEFT, RIGHT };

struct Bullet {
    SDL_Rect rect;
    int speed;
    bool active;
    Direction dir;
};

class Player {
public:
    Player();
    ~Player();

    void Init(SDL_Renderer* renderer);
    void HandleEvent(const SDL_Event& e);
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);

    SDL_Rect GetRect() const;
    SDL_Rect GetCollisionRect() const;

    std::vector<Bullet> GetBullets() const;
    std::vector<Bullet>& GetBulletsRef();

    void Shoot();
    void SetShootSound(Mix_Chunk* sound);
    void SetPosition(int newX, int newY);
    void SetWindowSize(int w, int h);

    bool doubleBullet = false;
    bool speedBoost = false;

private:
    int x, y;
    int w, h;
    int speed;
    int windowW, windowH;

    bool moveLeft, moveRight, moveUp, moveDown;
    Mix_Chunk* shootSound;
    SDL_Texture* texture;
    SDL_Texture* bulletTexture;

    std::vector<SDL_Texture*> runFrames;
    float frameTimer;
    int currentFrame;
    float frameDelay;

    std::vector<Bullet> bullets;
    Direction lastDir;
};

#endif
