#include "Viewport.h"
#include <algorithm>

namespace PvZ {

Viewport::Viewport(int windowWidth, int windowHeight)
    : mWindowWidth(windowWidth), mWindowHeight(windowHeight), mScale(1.0f) {
    mViewportRect = { 0, 0, (float)windowWidth, (float)windowHeight };
}

Viewport::~Viewport() {
    UnloadRenderTexture(mTargetBuffer);
    CloseWindow();
}

void Viewport::InitWindow(const char* title) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    ::InitWindow(mWindowWidth, mWindowHeight, title);
    SetWindowMinSize(960, 540);
    SetTargetFPS(60);

    mTargetBuffer = LoadRenderTexture((int)VIRTUAL_WIDTH, (int)VIRTUAL_HEIGHT);
    SetTextureFilter(mTargetBuffer.texture, TEXTURE_FILTER_BILINEAR);
}

void Viewport::Update() {
    mWindowWidth = GetScreenWidth();
    mWindowHeight = GetScreenHeight();

    mScale = std::min((float)mWindowWidth / VIRTUAL_WIDTH, (float)mWindowHeight / VIRTUAL_HEIGHT);

    float viewportWidth = VIRTUAL_WIDTH * mScale;
    float viewportHeight = VIRTUAL_HEIGHT * mScale;
    float offsetX = (mWindowWidth - viewportWidth) * 0.5f;
    float offsetY = (mWindowHeight - viewportHeight) * 0.5f;

    mViewportRect = { offsetX, offsetY, viewportWidth, viewportHeight };
}

void Viewport::BeginDrawing() {
    BeginTextureMode(mTargetBuffer);
    ClearBackground(BLACK);
}

void Viewport::EndDrawing() {
    EndTextureMode();

    ::BeginDrawing();
    ClearBackground(DARKGRAY);

    // Draw virtual buffer scaled and centered in window
    Rectangle sourceRect = { 0.0f, 0.0f, VIRTUAL_WIDTH, -VIRTUAL_HEIGHT }; // Negative height to flip Y in Raylib
    DrawTexturePro(mTargetBuffer.texture, sourceRect, mViewportRect, { 0.0f, 0.0f }, 0.0f, WHITE);

    ::EndDrawing();
}

Vector2 Viewport::GetVirtualMousePosition() const {
    Vector2 mousePos = GetMousePosition();
    Vector2 virtualPos = { 0, 0 };

    virtualPos.x = (mousePos.x - mViewportRect.x) / mScale;
    virtualPos.y = (mousePos.y - mViewportRect.y) / mScale;

    virtualPos.x = std::clamp(virtualPos.x, 0.0f, VIRTUAL_WIDTH);
    virtualPos.y = std::clamp(virtualPos.y, 0.0f, VIRTUAL_HEIGHT);

    return virtualPos;
}

} // namespace PvZ
