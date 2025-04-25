#include "enemy.h"
#include "maze.h"

Enemy::Enemy() : x(0), y(0), w(50), h(50), speedX(2.0f), direction(1), health(maxHealth), texture(nullptr) {}

void Enemy::Init(int x, int y, SDL_Texture* tex) {
    this->x = x;
    this->y = y;
    this->texture = tex;
    this->health = maxHealth;
}

void Enemy::Update(float deltaTime, const Maze& maze) {
    if (!IsAlive()) return;

    int nextX = x + static_cast<int>(speedX * 60.0f * deltaTime * direction);
    SDL_Rect nextRect = { nextX, y, w, h };

    bool blocked = false;

    if (nextX < 0 || nextX + w > Maze::MAP_COLS * Maze::TILE_SIZE) {
        blocked = true;
    }

    if (!blocked) {
        if (maze.IsWall(nextRect.y / Maze::TILE_SIZE, nextRect.x / Maze::TILE_SIZE) ||
            maze.IsWall((nextRect.y + nextRect.h - 1) / Maze::TILE_SIZE, nextRect.x / Maze::TILE_SIZE) ||
            maze.IsWall(nextRect.y / Maze::TILE_SIZE, (nextRect.x + nextRect.w - 1) / Maze::TILE_SIZE) ||
            maze.IsWall((nextRect.y + nextRect.h - 1) / Maze::TILE_SIZE, (nextRect.x + nextRect.w - 1) / Maze::TILE_SIZE)) {
            blocked = true;
        }
    }

    if (blocked) {
        direction *= -1;
    } else {
        x = nextX;
    }
}

void Enemy::Render(SDL_Renderer* renderer) {
    if (!IsAlive()) return;

    SDL_Rect dst = { x, y, w, h };
    SDL_RenderCopy(renderer, texture, nullptr, &dst);

    SDL_Rect bg = { x, y - 10, w, 6 };
    SDL_Rect hp = { x, y - 10, (int)(w * (health / (float)maxHealth)), 6 };
    SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bg);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &hp);
}

SDL_Rect Enemy::GetRect() const {
    return { x, y, w, h };
}

SDL_Rect Enemy::GetCollisionRect() const {
    int margin = 4;
    return { x + margin, y + margin, w - 2 * margin, h - 2 * margin };
}

void Enemy::TakeDamage(int dmg) {
    health -= dmg;
    if (health < 0) health = 0;
}

bool Enemy::IsAlive() const {
    return health > 0;
}

Enemy::~Enemy() {}
