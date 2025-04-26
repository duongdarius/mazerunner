#include "enemy.h"
#include "maze.h"

Enemy::Enemy() : x(0), y(0), w(48), h(48), speedX(2.0f), direction(1), health(4), maxHealth(4),
                 frameIndex(0), frameTimer(0.0f), frameDelay(0.15f), maxFrames(4), texture(nullptr) {}

void Enemy::Init(int x, int y, SDL_Texture* tex, int level) {
    this->x = x;
    this->y = y;
    this->texture = tex;
    this->frameIndex = 0;
    this->frameTimer = 0.0f;

    if (level == 2) {
        this->health = 10;
        this->maxHealth = 10;
        this->frameDelay = 0.1f;
        this->maxFrames = 6;
    } else {
        this->health = 4;
        this->maxHealth = 4;
        this->frameDelay = 0.15f;
        this->maxFrames = 4;
    }

    this->w = 48;
    this->h = 48;
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

    frameTimer += deltaTime;
    if (frameTimer >= frameDelay) {
        frameIndex = (frameIndex + 1) % maxFrames;
        frameTimer = 0.0f;
    }
}

void Enemy::Render(SDL_Renderer* renderer) {
    if (!IsAlive()) return;

    int frameWidth = 48;
    SDL_Rect src = { frameIndex * frameWidth, 0, frameWidth, 48 };

    int drawW, drawH;
    if (maxFrames == 4) {
        drawW = 48;
        drawH = 48;
    } else {
        drawW = 48;
        drawH = 48;
    }

    SDL_Rect dst = { x, y, drawW, drawH };
    SDL_RendererFlip flip = (direction == 1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer, texture, &src, &dst, 0.0, NULL, flip);

    SDL_Rect bg = { x, y - 10, drawW, 6 };
    SDL_Rect hp = { x, y - 10, (int)(drawW * (health / (float)maxHealth)), 6 };

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
