#pragma once
#include <windows.h>

class Camera {
public:
    Camera(float width, float height);
    ~Camera();

    void Update(float playerX, float playerY);
    float GetOffsetX() const;
    float GetOffsetY() const;

    void SetBounds(float width, float height); // 경계를 설정하는 메서드 추가

    bool IsRectInView(float x, float y, float w, float h,
        const RECT& clientRect, float margin = 200.0f) const;

    bool IsPointInView(float x, float y,
        const RECT& clientRect, float margin = 200.0f) const;

private:
    static bool IntersectAABB(float ax, float ay, float aw, float ah,
        float bx, float by, float bw, float bh);

private:
    float width, height;
    float offsetX, offsetY;
    float boundWidth, boundHeight; // 경계를 위한 변수
};
