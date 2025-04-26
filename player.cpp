#include "player.h"
#include <iostream>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

Player::Player() {
    x = 0;
    y = 0;
    w = 50;
    h = 50;
    speed = 5;
    moveLeft = moveRight = moveUp = moveDown = false;
    shootSound = nullptr;
    texture = nullptr;
    bulletTexture = nullptr;
    windowW = 800;
    windowH = 600;
    lastDir = RIGHT;
    frameTimer = 0.0f;
    currentFrame = 0;
    frameDelay = 0.1f;

    doubleBullet = false;
    speedBoost = false;
}

Player::~Player() {
    for (auto tex : runFrames) {
        SDL_DestroyTexture(tex);
    }
    if (bulletTexture) {
        SDL_DestroyTexture(bulletTexture);
    }
}

void Player::Init(SDL_Renderer* renderer) {
    for (int i = 0; i < 8; ++i) {
        std::string path = "C:\\Projects\\SDL\\MazeRunner\\assets\\player_run_" + std::to_string(i) + ".png";
        SDL_Surface* surf = IMG_Load(path.c_str());
        if (!surf) continue;
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);
        runFrames.push_back(tex);
    }

    SDL_Surface* surfBullet = IMG_Load("C:\\Projects\\SDL\\MazeRunner\\assets\\bullet.png");
    if (surfBullet) {
        bulletTexture = SDL_CreateTextureFromSurface(renderer, surfBullet);
        SDL_FreeSurface(surfBullet);
    }

    this->x = 0;
    this->y = 0;
    this->w = 64;
    this->h = 64;
    this->speed = 4;
    lastDir = RIGHT;
    moveLeft = moveRight = moveUp = moveDown = false;
}

void Player::HandleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && !e.key.repeat) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_a:
                moveLeft = true;
                lastDir = LEFT;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                moveRight = true;
                lastDir = RIGHT;
                break;
            case SDLK_UP:
            case SDLK_w:
                moveUp = true;
                break;
            case SDLK_DOWN:
            case SDLK_s:
                moveDown = true;
                break;
            case SDLK_SPACE:
                Shoot();
                break;
            default:
                break;
        }
    } else if (e.type == SDL_KEYUP && !e.key.repeat) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_a:
                moveLeft = false;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                moveRight = false;
                break;
            case SDLK_UP:
            case SDLK_w:
                moveUp = false;
                break;
            case SDLK_DOWN:
            case SDLK_s:
                moveDown = false;
                break;
            default:
                break;
        }
    }
}

void Player::Update(float deltaTime) {
    int moveX = 0, moveY = 0;
    if (moveLeft)  moveX -= 1;
    if (moveRight) moveX += 1;
    if (moveUp)    moveY -= 1;
    if (moveDown)  moveY += 1;

    float currentSpeed = speedBoost ? speed * 1.5f : speed;
    float spf = currentSpeed * 60.0f * deltaTime;
    x += (int)(moveX * spf);
    y += (int)(moveY * spf);

    if (x < 0) x = 0;
    if (x + w > windowW) x = windowW - w;
    if (y < 0) y = 0;
    if (y + h > windowH) y = windowH - h;

    frameTimer += deltaTime;
    if (frameTimer >= frameDelay) {
        frameTimer = 0.0f;
        currentFrame = (currentFrame + 1) % runFrames.size();
    }

    for (auto& b : bullets) {
        if (!b.active) continue;

        if (b.dir == LEFT)  b.rect.x -= b.speed;
        else                b.rect.x += b.speed;

        if (b.rect.x < 0 || b.rect.x > windowW)
            b.active = false;
    }
}

void Player::Render(SDL_Renderer* renderer) {
    SDL_Rect dst = { x, y, w, h };
    if (!runFrames.empty()) {
        SDL_RendererFlip flip = (lastDir == LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_RenderCopyEx(renderer, runFrames[currentFrame], nullptr, &dst, 0, nullptr, flip);
    } else if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &dst);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &dst);
    }

    for (auto& b : bullets) {
        if (b.active) {
            if (bulletTexture)
                SDL_RenderCopy(renderer, bulletTexture, nullptr, &b.rect);
            else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderFillRect(renderer, &b.rect);
            }
        }
    }
}

void Player::Shoot() {
    if (doubleBullet) {
        for (int i = 0; i < 2; ++i) {
            Bullet b;
            b.speed = 10;
            b.active = true;
            b.dir = lastDir;
            b.rect.w = 32;
            b.rect.h = 12;
            b.rect.y = y + h / 2 - b.rect.h / 2 + 8;

            int delayOffset = i * 30;

            if (lastDir == LEFT) {
                b.rect.x = x - delayOffset;
            } else {
                b.rect.x = x + w - b.rect.w + delayOffset;
            }

            bullets.push_back(b);
        }
    } else {
        Bullet b;
        b.speed = 10;
        b.active = true;
        b.dir = lastDir;
        b.rect.w = 32;
        b.rect.h = 12;
        b.rect.y = y + h / 2 - b.rect.h / 2 + 8;
        b.rect.x = (lastDir == LEFT) ? x : (x + w - b.rect.w);
        bullets.push_back(b);
    }

    if (shootSound)
        Mix_PlayChannel(-1, shootSound, 0);
}

std::vector<Bullet> Player::GetBullets() const {
    return bullets;
}

std::vector<Bullet>& Player::GetBulletsRef() {
    return bullets;
}

SDL_Rect Player::GetRect() const {
    return { x, y, w, h };
}

SDL_Rect Player::GetCollisionRect() const {
    int marginX = 24;
    int marginY = 20;
    return { x + marginX, y + marginY, w - 2 * marginX, h - 2 * marginY };
}

void Player::SetShootSound(Mix_Chunk* sound) {
    shootSound = sound;
}

void Player::SetPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

void Player::SetWindowSize(int w, int h) {
    windowW = w;
    windowH = h;
}
