#include "player.h"
#include <iostream>

Player::Player()
{
    // Toạ độ ban đầu
    x = 0;
    y = 0;
    // Kích thước giả sử 50×50
    w = 50;
    h = 50;
    // Tốc độ cơ bản (bạn tự điều chỉnh)
    speed = 5;
    texture = nullptr;

    // Ban đầu không di chuyển
    moveLeft = moveRight = moveUp = moveDown = false;
}

Player::~Player()
{
    // Không huỷ texture tại đây (main sẽ quản lý SDL_DestroyTexture)
}

void Player::Init(int x, int y, SDL_Texture* tex)
{
    this->x = x;
    this->y = y;
    this->w = 50;
    this->h = 50;
    this->speed = 5;  // Tùy ý
    this->texture = tex;

    // Mới khởi tạo => cờ di chuyển = false
    moveLeft = moveRight = moveUp = moveDown = false;
}

void Player::HandleEvent(const SDL_Event& e)
{
    // Khi ấn phím xuống (và không bị repeat)
    if (e.type == SDL_KEYDOWN && !e.key.repeat)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT:
        case SDLK_a:
            moveLeft = true;
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            moveRight = true;
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
            // Bắn đạn 1 lần mỗi lần ấn Space
            Shoot();
            break;
        default:
            break;
        }
    }
    // Khi nhả phím
    else if (e.type == SDL_KEYUP && !e.key.repeat)
    {
        switch (e.key.keysym.sym)
        {
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

void Player::Update(float deltaTime)
{
    // Tính hướng di chuyển
    int moveX = 0;
    int moveY = 0;

    if (moveLeft)  moveX -= 1;
    if (moveRight) moveX += 1;
    if (moveUp)    moveY -= 1;
    if (moveDown)  moveY += 1;

    // Di chuyển theo deltaTime
    // Ví dụ speed=5, deltaTime*(60) => di chuyển 5 px/f
    float speedPerFrame = speed * deltaTime * 60;
    x += (int)(moveX * speedPerFrame);
    y += (int)(moveY * speedPerFrame);

    // Giữ Player trong màn hình 800×600 (bạn điều chỉnh tuỳ game)
    if (x < 0) x = 0;
    if (x + w > 800) x = 800 - w;
    if (y < 0) y = 0;
    if (y + h > 600) y = 600 - h;

    // Cập nhật đạn (bay lên trên)
    for (auto &b : bullets)
    {
        if (b.active)
        {
            b.rect.y -= b.speed;
            // Nếu ra khỏi màn hình
            if (b.rect.y + b.rect.h < 0)
            {
                b.active = false;
            }
        }
    }
}

void Player::Render(SDL_Renderer* renderer)
{
    // Vẽ Player
    SDL_Rect dst = { x, y, w, h };
    if (texture)
    {
        SDL_RenderCopy(renderer, texture, NULL, &dst);
    }
    else
    {
        // Nếu không có ảnh => vẽ ô màu xanh
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &dst);
    }

    // Vẽ đạn (màu vàng)
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for (auto &b : bullets)
    {
        if (b.active)
        {
            SDL_RenderFillRect(renderer, &b.rect);
        }
    }
}

SDL_Rect Player::GetRect() const
{
    return { x, y, w, h };
}

std::vector<Bullet> Player::GetBullets() const
{
    return bullets;
}

// Hàm bắn 1 viên đạn
void Player::Shoot()
{
    Bullet b;
    b.rect.w = 8;
    b.rect.h = 16;
    // Đặt đạn ở giữa đầu Player
    b.rect.x = x + w/2 - b.rect.w/2;
    b.rect.y = y;
    b.speed  = 10;  // tốc độ bay
    b.active = true;

    bullets.push_back(b);
}
