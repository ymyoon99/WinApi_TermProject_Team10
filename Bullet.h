#pragma once
#include <atlimage.h> 
#include <vector>
#include <cmath>

enum class BulletKind {
    Revolver,
    Headshot,
    Cluster,
    DualShotgun
};

class Bullet {
public:
    virtual ~Bullet();

    void Update(float frameTime);
    void Draw(HDC hdc, float offsetX, float offsetY);
    bool IsOutOfBounds(float width, float height) const;
    bool CheckCollision(float enemyX, float enemyY, float enemyWidth, float enemyHeight) const;
    int GetDamage() const;

    bool isHit;
    bool isEffectFinished() const;

    BulletKind GetKind() const { return kind; }
    virtual void Reset(float startX, float startY, float targetX, float targetY);

protected:
    // 기본 생성자(풀에서 new할 때 사용)
    Bullet(BulletKind kind, int damage, float speed);

    // 총알 상태
    float x = 0.0f, y = 0.0f;
    float speed = 0.0f;
    float directionX = 1.0f, directionY = 0.0f;
    int damage = 0;
    BulletKind kind;

    const CImage* bulletImage;

    float hitEffectDuration = 0.25f;
    float hitEffectTime = 0.0f;
    std::vector<const CImage*> hitEffectImages;

private:
    void UpdateHitEffect(float frameTime);
    void DrawHitEffect(HDC hdc, float offsetX, float offsetY);
};

class RevolverBullet : public Bullet {
public:
    RevolverBullet();
};

class HeadshotGunBullet : public Bullet {
public:
    HeadshotGunBullet();
};

class ClusterGunBullet : public Bullet {
public:
    ClusterGunBullet();
};

class DualShotgunBullet : public Bullet {
public:
    DualShotgunBullet();

    // 퍼짐각 적용 Reset
    void ResetWithSpread(float startX, float startY, float targetX, float targetY, float spreadRad);
};
