#pragma once
#include "raylib.h"
#include "../ecs/EntityManager.h"

namespace PvZ {

class SunManager {
public:
    static SunManager& Instance();

    void Init();
    void Update(float dt, Vector2 mousePos, bool mouseClicked);
    void Render();

    int GetSun() const { return mSunCount; }
    void AddSun(int amount) { mSunCount += amount; }
    bool SpendSun(int amount);

    void SpawnSun(Vector2 startPos, Vector2 targetPos, int amount = 25);

private:
    SunManager() = default;
    ~SunManager() = default;

    int mSunCount = 150;
    float mSkySunTimer = 0.0f;
    const float SKY_SUN_INTERVAL = 10.0f;
};

} // namespace PvZ
