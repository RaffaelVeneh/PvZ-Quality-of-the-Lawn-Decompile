#pragma once
#include "raylib.h"

namespace PvZ {

class Viewport {
public:
    static constexpr float VIRTUAL_WIDTH = 1920.0f;
    static constexpr float VIRTUAL_HEIGHT = 1080.0f;

    Viewport(int windowWidth = 1280, int windowHeight = 720);
    ~Viewport();

    void InitWindow(const char* title);
    void Update();

    void BeginDrawing();
    void EndDrawing();

    Vector2 GetVirtualMousePosition() const;
    float GetScale() const { return mScale; }
    Rectangle GetViewportRect() const { return mViewportRect; }

private:
    int mWindowWidth;
    int mWindowHeight;
    RenderTexture2D mTargetBuffer;
    Rectangle mViewportRect;
    float mScale;
};

} // namespace PvZ
