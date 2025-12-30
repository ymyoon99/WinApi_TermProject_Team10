#include "Camera.h"

Camera::Camera(float width, float height) : width(width), height(height), offsetX(0), offsetY(0), boundWidth(0), boundHeight(0) {
}

Camera::~Camera() {
}

void Camera::Update(float playerX, float playerY) {
    offsetX = playerX - width / 2;
    offsetY = playerY - height / 2;

    // 경계를 벗어나지 않도록 위치 제한
    if (offsetX < 0) offsetX = 0;
    if (offsetY < 0) offsetY = 0;
    if (offsetX > boundWidth - width) offsetX = boundWidth - width;
    if (offsetY > boundHeight - height) offsetY = boundHeight - height;
}

void Camera::SetBounds(float width, float height) {
    boundWidth = width;
    boundHeight = height;
}

float Camera::GetOffsetX() const {
    return offsetX;
}

float Camera::GetOffsetY() const {
    return offsetY;
}

bool Camera::IntersectAABB(float ax, float ay, float aw, float ah,
    float bx, float by, float bw, float bh)
{
    return (ax < bx + bw) && (ax + aw > bx) && (ay < by + bh) && (ay + ah > by);
}

bool Camera::IsRectInView(float x, float y, float w, float h,
    const RECT& clientRect, float margin) const
{
    const float viewW = static_cast<float>(clientRect.right - clientRect.left);
    const float viewH = static_cast<float>(clientRect.bottom - clientRect.top);

    // 카메라 월드 좌상단(=offset)
    const float vx = offsetX - margin;
    const float vy = offsetY - margin;
    const float vw = viewW + margin * 2.0f;
    const float vh = viewH + margin * 2.0f;

    return IntersectAABB(x, y, w, h, vx, vy, vw, vh);
}

bool Camera::IsPointInView(float x, float y,
    const RECT& clientRect, float margin) const
{
    const float viewW = static_cast<float>(clientRect.right - clientRect.left);
    const float viewH = static_cast<float>(clientRect.bottom - clientRect.top);

    const float vx = offsetX - margin;
    const float vy = offsetY - margin;
    const float vw = viewW + margin * 2.0f;
    const float vh = viewH + margin * 2.0f;

    return (x >= vx && x <= vx + vw && y >= vy && y <= vy + vh);
}