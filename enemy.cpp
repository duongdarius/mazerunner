#include "enemy.h"
#include <iostream>

Enemy::Enemy()
{
    x = 0;
    y = 0;
    w = 50;
    h = 50;
    speedX = 2.0f;
    texture = nullptr;
    isAlive = true;

    startX = 0;
    moveRange = 128;
    direction = 1;
}

Enemy::~Enemy()
{
    // Không hủy texture vì texture được quản lý ở main
}

void Enemy::Init(int x, int y, SDL_Texture* tex)
{
    this->x = x;
    this->y = y;
    this->w = 50;
    this->h = 50;
    this->speedX = 2.0f;
    this->texture = tex;
    this->isAlive = true;

    startX = x;
    moveRange = 128;
    direction = 1;
}

void Enemy::Update(float deltaTime)
{
    if(!isAlive) return;

    float dist = speedX * 60.0f * deltaTime;
    x += (int)(direction * dist);

    // Giới hạn di chuyển [startX, startX+moveRange]
    if(x < startX){
        x = startX;
        direction = 1;
    }
    else if(x > startX + moveRange){
        x = startX + moveRange;
        direction = -1;
    }
}

void Enemy::Render(SDL_Renderer* renderer)
{
    if(!isAlive) return;

    SDL_Rect dst = { x, y, w, h };
    if(texture){
        SDL_RenderCopy(renderer, texture, nullptr, &dst);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &dst);
    }
}

SDL_Rect Enemy::GetRect() const
{
    return { x, y, w, h };
}

// --- Mới: Lấy bounding box thu nhỏ với margin ---
SDL_Rect Enemy::GetCollisionRect() const
{
    int margin = 10;  // Điều chỉnh margin theo trực quan của sprite
    return { x + margin, y + margin, w - 2 * margin, h - 2 * margin };
}
