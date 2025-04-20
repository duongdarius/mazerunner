#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>

// Enemy di chuyển qua lại 2 ô
class Enemy
{
public:
    Enemy();
    ~Enemy();

    void Init(int x, int y, SDL_Texture* tex);
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);

    SDL_Rect GetRect() const;
    // --- Mới: Hàm lấy bounding box thu nhỏ ---
    SDL_Rect GetCollisionRect() const;

    bool isAlive;

private:
    int x, y;
    int w, h;
    float speedX;
    SDL_Texture* texture;

    int startX;
    int moveRange; // 128 = 2 ô
    int direction; // 1=phải, -1=trái
};

#endif
