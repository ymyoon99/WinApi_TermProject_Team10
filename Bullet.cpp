#include "Bullet.h"
#include <cmath>

namespace {
    const CImage* GetSharedBulletImage()
    {
        static CImage s_img;
        static bool s_loaded = false;

        if (!s_loaded) {
            s_img.Load(L"./resources/gun/SmallCircle.png");
            s_loaded = true;
        }
        return &s_img;
    }

    const std::vector<CImage>& GetSharedHitEffectImages()
    {
        static std::vector<CImage> s_imgs;
        static bool s_loaded = false;

        if (!s_loaded) {
            s_imgs.resize(2);
            s_imgs[0].Load(L"./resources/effect/HitImpactFX_0.png");
            s_imgs[1].Load(L"./resources/effect/HitImpactFX_1.png");
            s_loaded = true;
        }
        return s_imgs;
    }
}

Bullet::Bullet(BulletKind kind, int damage, float speed)
    : speed(speed), damage(damage), kind(kind)
{
    // 리소스 연결
    bulletImage = GetSharedBulletImage();

    const auto& sharedHit = GetSharedHitEffectImages();
    hitEffectImages.clear();
    hitEffectImages.reserve(sharedHit.size());
    for (const auto& img : sharedHit) {
        hitEffectImages.push_back(&img);
    }

    // 안전 기본값
    x = y = 0.0f;
    directionX = 1.0f;
    directionY = 0.0f;
    isHit = false;
    hitEffectTime = 0.0f;
}

Bullet::~Bullet() {

}

void Bullet::Reset(float startX, float startY, float targetX, float targetY)
{
    x = startX;
    y = startY;

    isHit = false;
    hitEffectTime = 0.0f;

    float dx = targetX - startX;
    float dy = targetY - startY;
    float distSq = dx * dx + dy * dy;

    // 0으로 나누기 방지
    if (distSq < 0.000001f) {
        directionX = 1.0f;
        directionY = 0.0f;
        return;
    }

    float dist = std::sqrt(distSq);
    directionX = dx / dist;
    directionY = dy / dist;
}

void Bullet::Update(float frameTime)
{
    if (isHit) {
        UpdateHitEffect(frameTime);
    }
    else {
        x += directionX * speed * frameTime;
        y += directionY * speed * frameTime;
    }
}

void Bullet::UpdateHitEffect(float frameTime) 
{
    hitEffectTime += frameTime;
}

void Bullet::Draw(HDC hdc, float offsetX, float offsetY) 
{
    if (isHit) {
        DrawHitEffect(hdc, offsetX, offsetY);
    }
    else {
        if (bulletImage && !bulletImage->IsNull()) {
            bulletImage->Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
        }
    }
}

void Bullet::DrawHitEffect(HDC hdc, float offsetX, float offsetY) 
{
    if (hitEffectImages.empty()) return;

    int frame = static_cast<int>((hitEffectTime / hitEffectDuration) * hitEffectImages.size());
    if (frame >= (int)hitEffectImages.size()) {
        frame = (int)hitEffectImages.size() - 1;
    }

    const CImage* img = hitEffectImages[frame];
    if (img && !img->IsNull()) {
        img->Draw(hdc, static_cast<int>(x - offsetX), static_cast<int>(y - offsetY));
    }
}

bool Bullet::IsOutOfBounds(float width, float height) const 
{
    return x < 0 || y < 0 || x > width || y > height;
}

bool Bullet::CheckCollision(float enemyX, float enemyY, float enemyWidth, float enemyHeight) const 
{
    return x > enemyX && x < enemyX + enemyWidth &&
        y > enemyY && y < enemyY + enemyHeight;
}

int Bullet::GetDamage() const 
{
    return damage;
}

bool Bullet::isEffectFinished() const 
{
    return hitEffectTime >= hitEffectDuration;
}

// 파생 총알 클래스
RevolverBullet::RevolverBullet()
    : Bullet(BulletKind::Revolver, 50, 1500.0f)
{
}

HeadshotGunBullet::HeadshotGunBullet()
    : Bullet(BulletKind::Headshot, 100, 1500.0f)
{
}

ClusterGunBullet::ClusterGunBullet()
    : Bullet(BulletKind::Cluster, 75, 1500.0f)
{
}

DualShotgunBullet::DualShotgunBullet()
    : Bullet(BulletKind::DualShotgun, 100, 1500.0f)
{
}

void DualShotgunBullet::ResetWithSpread(float startX, float startY, float targetX, float targetY, float spreadRad)
{
    // 기본 방향 먼저 계산
    Reset(startX, startY, targetX, targetY);

    // 그 방향을 spreadRad 만큼 회전
    float ox = directionX;
    float oy = directionY;

    float cs = std::cos(spreadRad);
    float sn = std::sin(spreadRad);

    directionX = ox * cs - oy * sn;
    directionY = ox * sn + oy * cs;
}