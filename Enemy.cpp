#include "Enemy.h"
#include "GameFrameWork.h"
#include <cmath>

#define EnemyWidth 50.0f
#define EnemyHeight 50.0f

Enemy::Enemy(float x, float y, float speed, int health, float animationSpeed, float eWidth, float eHeight)
    : x(x), y(y), speed(speed), health(health), boundWidth(0), boundHeight(0), currentFrame(0), frameTimeAccumulator(0.0f), animationSpeed(animationSpeed), eWidth(eWidth), eHeight(eHeight),
    isDying(false), deathEffectDuration(0.5f), deathEffectStart(0.0f) {
    LoadImages();

    // Load death effect images
    static bool s_deathLoaded = false;
    static std::vector<CImage> s_death;

    if (!s_deathLoaded) {
        s_death.resize(4);
        s_death[0].Load(L"./resources/effect/DeathFX_0.png");
        s_death[1].Load(L"./resources/effect/DeathFX_1.png");
        s_death[2].Load(L"./resources/effect/DeathFX_2.png");
        s_death[3].Load(L"./resources/effect/DeathFX_3.png");
        s_deathLoaded = true;
    }

    deathEffectImages.clear();
    deathEffectImages.reserve(s_death.size());
    for (auto& img : s_death) {
        deathEffectImages.push_back(&img);
    }

}

Enemy::~Enemy() {
}

void Enemy::LoadImages() {
    idleImages.resize(5);
}


void Enemy::Update(float frameTime, float playerX, float playerY, const std::vector<Obstacle*>& obstacles) {

    if (isDying) {
        UpdateDeathEffect(frameTime);
        return;
    }

    float dx = playerX - x;
    float dy = playerY - y;
    float distance = sqrt(dx * dx + dy * dy);

    float directionX = dx / distance;
    float directionY = dy / distance;

    float newX = x + directionX * speed * frameTime;
    float newY = y + directionY * speed * frameTime;

    if (!CheckCollision(newX, newY, obstacles)) {
        x = newX;
        y = newY;
    }

    frameTimeAccumulator += frameTime;
    if (frameTimeAccumulator >= animationSpeed) {

        currentFrame = (currentFrame + 1) % idleImages.size();

        frameTimeAccumulator = 0.0f;
    }
}

void Enemy::UpdateDeathEffect(float frameTime) {
    deathEffectStart += frameTime;
    if (deathEffectStart >= deathEffectDuration) {
        isDying = false;
        deathEffectStart = 0.0f;
    }
}

bool Enemy::CheckCollision(float newX, float newY, const std::vector<Obstacle*>& obstacles) const {
    for (const auto& obstacle : obstacles) {
        float ox = obstacle->GetX();
        float oy = obstacle->GetY();
        float ow = obstacle->GetWidth();
        float oh = obstacle->GetHeight();

        if (newX < ox + ow &&
            newX + eWidth > ox &&
            newY < oy + oh &&
            newY + eHeight > oy) {

            return true;
        }
    }
    return false;
}

void Enemy::Draw(HDC hdc, float offsetX, float offsetY) {

    if (isDying) {
        DrawDeathEffect(hdc, offsetX, offsetY);
        return;
    }

    if (idleImages.empty()) return;

    const CImage* img = idleImages[currentFrame];
    if (img && !img->IsNull()) {
        img->Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
    }
}

void Enemy::DrawDeathEffect(HDC hdc, float offsetX, float offsetY) {
    int frame = static_cast<int>((deathEffectStart / deathEffectDuration) * deathEffectImages.size());

    if(frame >= 0 && frame < (int)deathEffectImages.size()) {
        const CImage* img = deathEffectImages[frame];
        if (img && !img->IsNull()) {
            img->Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
        }
    }
}

void Enemy::TakeDamage(int damage) {
    health -= damage;
    if (health <= 0 && !isDying) {
        isDying = true;
        health = 0;
        deathEffectStart = 0.0f;
    }
}

bool Enemy::IsDead() const {
    return health <= 0 && !isDying;
}

float Enemy::GetX() const {
    return x;
}

float Enemy::GetY() const {
    return y;
}

float Enemy::GetWidth() const {
    return eWidth;
}

float Enemy::GetHeight() const {
    return eHeight;
}

// BrainMonster
BrainMonster::BrainMonster(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, 20.0f, health, 0.2f, eWidth, eHeight) {

    LoadImages();
}

void BrainMonster::LoadImages() {
    static bool s_loaded = false;
    static std::vector<CImage> s_imgs;

    if (!s_loaded) {
        s_imgs.resize(4);
        s_imgs[0].Load(L"./resources/enemy/BrainMonster_0.png");
        s_imgs[1].Load(L"./resources/enemy/BrainMonster_1.png");
        s_imgs[2].Load(L"./resources/enemy/BrainMonster_2.png");
        s_imgs[3].Load(L"./resources/enemy/BrainMonster_3.png");
        s_loaded = true;
    }

    idleImages.clear();
    idleImages.reserve(s_imgs.size());
    for (auto& img : s_imgs) idleImages.push_back(&img);
}

// EyeMonster
EyeMonster::EyeMonster(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, 40.0f, health, 0.2f, eWidth, eHeight) {

    LoadImages();
}

void EyeMonster::LoadImages() {
    static bool s_loaded = false;
    static std::vector<CImage> s_imgs;

    if (!s_loaded) {
        s_imgs.resize(3);
        s_imgs[0].Load(L"./resources/enemy/EyeMonster_0.png");
        s_imgs[1].Load(L"./resources/enemy/EyeMonster_1.png");
        s_imgs[2].Load(L"./resources/enemy/EyeMonster_2.png");
        s_loaded = true;
    }

    idleImages.clear();
    idleImages.reserve(s_imgs.size());
    for (auto& img : s_imgs) idleImages.push_back(&img);
}

// BigBoomer
BigBoomer::BigBoomer(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, 30.0f, health, 0.2f, eWidth, eHeight) {

    LoadImages();
}

void BigBoomer::LoadImages() {
    static bool s_loaded = false;
    static std::vector<CImage> s_imgs;

    if (!s_loaded) {
        s_imgs.resize(4);
        s_imgs[0].Load(L"./resources/enemy/BigBoomer_0.png");
        s_imgs[1].Load(L"./resources/enemy/BigBoomer_1.png");
        s_imgs[2].Load(L"./resources/enemy/BigBoomer_2.png");
        s_imgs[3].Load(L"./resources/enemy/BigBoomer_3.png");
        s_loaded = true;
    }

    idleImages.clear();
    idleImages.reserve(s_imgs.size());
    for (auto& img : s_imgs) idleImages.push_back(&img);
}

// Lamprey
Lamprey::Lamprey(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, 40.0f, health, 0.2f, eWidth, eHeight) {

    LoadImages();
}

void Lamprey::LoadImages() {
    static bool s_loaded = false;
    static std::vector<CImage> s_imgs;

    if (!s_loaded) {
        s_imgs.resize(5);
        s_imgs[0].Load(L"./resources/enemy/T_Lamprey_0.png");
        s_imgs[1].Load(L"./resources/enemy/T_Lamprey_1.png");
        s_imgs[2].Load(L"./resources/enemy/T_Lamprey_2.png");
        s_imgs[3].Load(L"./resources/enemy/T_Lamprey_3.png");
        s_imgs[4].Load(L"./resources/enemy/T_Lamprey_4.png");
        s_loaded = true;
    }

    idleImages.clear();
    idleImages.reserve(s_imgs.size());
    for (auto& img : s_imgs) idleImages.push_back(&img);
}

// Yog
Yog::Yog(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, 50.0f, health, 0.2f, eWidth, eHeight) {

    LoadImages();
}

void Yog::LoadImages() {
    static bool s_loaded = false;
    static std::vector<CImage> s_imgs;

    if (!s_loaded) {
        s_imgs.resize(4);
        s_imgs[0].Load(L"./resources/enemy/T_Yog_0.png");
        s_imgs[1].Load(L"./resources/enemy/T_Yog_1.png");
        s_imgs[2].Load(L"./resources/enemy/T_Yog_2.png");
        s_imgs[3].Load(L"./resources/enemy/T_Yog_3.png");
        s_loaded = true;
    }

    idleImages.clear();
    idleImages.reserve(s_imgs.size());
    for (auto& img : s_imgs) idleImages.push_back(&img);
}

// Boss
BossYog::BossYog(float x, float y, float speed, int health, float eWidth, float eHeight)
    : Enemy(x, y, speed, health, 1.0f, eWidth, eHeight),
    isPreparing(false),
    isCharging(false),
    prepareTime(0.0f),
    prepareDuration(1.0f), // 준비 동작 지속 시간
    chargeSpeed(300.0f),
    chargeCooldown(4.0f), // 돌진 주기를 4초로 설정
    chargeTimer(0.0f),
    chargeDistance(0.0f), // 돌진 거리 초기화
    prepareFrame(0),
    prepareFrameTime(0.0f),
    prepareFrameDuration(0.1f) { // 준비 동작 프레임 속도

    LoadImages();
}

void BossYog::LoadImages() {
    static bool s_loaded = false;
    static std::vector<CImage> s_idle;

    if (!s_loaded) {
        s_idle.resize(5);
        s_idle[0].Load(L"./resources/enemy/WingedMonster_0.png");
        s_idle[1].Load(L"./resources/enemy/WingedMonster_1.png");
        s_idle[2].Load(L"./resources/enemy/WingedMonster_2.png");
        s_idle[3].Load(L"./resources/enemy/WingedMonster_3.png");
        s_idle[4].Load(L"./resources/enemy/WingedMonster_4.png");
        s_loaded = true;
    }

    idleImages.clear();
    idleImages.reserve(s_idle.size());
    for (auto& img : s_idle) idleImages.push_back(&img);

    // 준비 동작 이미지를 로드
    prepareImages.resize(6);
    prepareImages[0].Load(L"./resources/effect/T_FireExplosionSmall_0.png");
    prepareImages[1].Load(L"./resources/effect/T_FireExplosionSmall_1.png");
    prepareImages[2].Load(L"./resources/effect/T_FireExplosionSmall_2.png");
    prepareImages[3].Load(L"./resources/effect/T_FireExplosionSmall_3.png");
    prepareImages[4].Load(L"./resources/effect/T_FireExplosionSmall_4.png");
    prepareImages[5].Load(L"./resources/effect/T_FireExplosionSmall_5.png");

    // 히트박스를 이미지 크기에 맞춰 조정
    if (!s_idle.empty() && !s_idle[0].IsNull()) {
        eWidth = static_cast<float>(s_idle[0].GetWidth());
        eHeight = static_cast<float>(s_idle[0].GetHeight());
    }
}

void BossYog::Update(float frameTime, float playerX, float playerY, const std::vector<Obstacle*>& obstacles) {
    chargeTimer += frameTime;

    if (isPreparing) {
        prepareTime += frameTime;
        prepareFrameTime += frameTime;

        if (prepareFrameTime >= prepareFrameDuration) {
            prepareFrame = (prepareFrame + 1) % prepareImages.size();
            prepareFrameTime = 0.0f;
        }

        if (prepareTime >= prepareDuration) {
            isPreparing = false;
            isCharging = true;
            prepareTime = 0.0f;

            float dx = playerX - x;
            float dy = playerY - y;
            float distance = sqrt(dx * dx + dy * dy);
            chargeDistance = distance; // 플레이어와의 간격만큼 돌진
        }
    }
    else if (isCharging) {
        float dx = playerX - x;
        float dy = playerY - y;
        float distance = sqrt(dx * dx + dy * dy);

        float directionX = dx / distance;
        float directionY = dy / distance;

        float newX = x + directionX * chargeSpeed * frameTime;
        float newY = y + directionY * chargeSpeed * frameTime;

        if (!CheckCollision(newX, newY, obstacles)) {
            x = newX;
            y = newY;
            chargeDistance -= chargeSpeed * frameTime;
        }

        if (chargeDistance <= 0.0f) { // 돌진 거리가 0 이하가 되면 돌진 멈춤
            isCharging = false;
            chargeTimer = 0.0f;
        }
    }
    else {
        float dx = playerX - x;
        float dy = playerY - y;
        float distance = sqrt(dx * dx + dy * dy);

        if (chargeTimer >= chargeCooldown) {
            isPreparing = true;
            chargeTimer = 0.0f;
        }
        else {
            float directionX = dx / distance;
            float directionY = dy / distance;

            float newX = x + directionX * speed * frameTime;
            float newY = y + directionY * speed * frameTime;

            if (!CheckCollision(newX, newY, obstacles)) {
                x = newX;
                y = newY;
            }

            frameTimeAccumulator += frameTime;
            if (frameTimeAccumulator >= animationSpeed) {
                currentFrame = (currentFrame + 1) % idleImages.size();
                frameTimeAccumulator = 0.0f;
            }
        }
    }
}

void BossYog::Draw(HDC hdc, float offsetX, float offsetY) {
    if (isPreparing && !prepareImages[prepareFrame].IsNull()) {
        prepareImages[prepareFrame].Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
    }
    else if (!idleImages.empty() && idleImages[currentFrame] && !idleImages[currentFrame]->IsNull()) {
        idleImages[currentFrame]->Draw(hdc, (int)(x - offsetX), (int)(y - offsetY));
    }
}

