#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>

class Maze;

class Enemy {
public:
    Enemy();
    ~Enemy();

    void Init(int x, int y, SDL_Texture* tex);
    void Update(float deltaTime, const Maze& maze);
    void Render(SDL_Renderer* renderer);

    SDL_Rect GetRect() const;
    SDL_Rect GetCollisionRect() const;

    void TakeDamage(int dmg);
    bool IsAlive() const;

private:
    int x, y;
    int w, h;
    float speedX;
    int direction;
    int health;
    static const int maxHealth = 4;
    SDL_Texture* texture;
};

#endif
