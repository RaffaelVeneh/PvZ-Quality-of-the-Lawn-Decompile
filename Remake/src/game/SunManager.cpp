#include "SunManager.h"
#include "../ecs/Component.h"
#include <cmath>

namespace PvZ {

SunManager& SunManager::Instance() {
    static SunManager instance;
    return instance;
}

void SunManager::Init() {
    mSunCount = 150;
    mSkySunTimer = 0.0f;
}

bool SunManager::SpendSun(int amount) {
    if (mSunCount >= amount) {
        mSunCount -= amount;
        return true;
    }
    return false;
}

void SunManager::SpawnSun(Vector2 startPos, Vector2 targetPos, int amount) {
    auto entity = EntityManager::Instance().CreateEntity(EntityTag::SUN);
    auto& pos = entity->AddComponent<PositionComponent>();
    pos.x = startPos.x;
    pos.y = startPos.y;

    auto& vel = entity->AddComponent<VelocityComponent>();
    vel.vx = (targetPos.x - startPos.x) / 2.0f;
    vel.vy = (targetPos.y - startPos.y) / 2.0f;

    auto& render = entity->AddComponent<RenderComponent>();
    render.tint = YELLOW;
}

void SunManager::Update(float dt, Vector2 mousePos, bool mouseClicked) {
    // Sky sun spawner
    mSkySunTimer += dt;
    if (mSkySunTimer >= SKY_SUN_INTERVAL) {
        mSkySunTimer = 0.0f;
        float startX = (float)GetRandomValue(350, 1500);
        float targetY = (float)GetRandomValue(250, 800);
        SpawnSun({ startX, -50.0f }, { startX, targetY }, 25);
    }

    // Update Sun Entities
    auto sunEntities = EntityManager::Instance().GetEntitiesByTag(EntityTag::SUN);
    for (auto& entity : sunEntities) {
        auto pos = entity->GetComponent<PositionComponent>();
        auto vel = entity->GetComponent<VelocityComponent>();

        if (pos && vel) {
            pos->x += vel->vx * dt;
            pos->y += vel->vy * dt;

            // Decelerate movement towards target pos
            vel->vx *= (1.0f - 2.0f * dt);
            vel->vy *= (1.0f - 2.0f * dt);

            // Sun collection hit test
            float distSq = (mousePos.x - pos->x) * (mousePos.x - pos->x) + (mousePos.y - pos->y) * (mousePos.y - pos->y);
            if (mouseClicked && distSq <= 45.0f * 45.0f) {
                AddSun(25);
                entity->Destroy();
            }
        }
    }
}

void SunManager::Render() {
    auto sunEntities = EntityManager::Instance().GetEntitiesByTag(EntityTag::SUN);
    for (auto& entity : sunEntities) {
        auto pos = entity->GetComponent<PositionComponent>();
        if (pos) {
            DrawCircle((int)pos->x, (int)pos->y, 35.0f, GOLD);
            DrawCircle((int)pos->x, (int)pos->y, 28.0f, YELLOW);
            DrawText("25", (int)pos->x - 10, (int)pos->y - 10, 20, BROWN);
        }
    }

    // Draw Sun Bank HUD
    DrawRectangle(50, 40, 260, 90, BROWN);
    DrawRectangleLinesEx({ 50, 40, 260, 90 }, 4.0f, DARKBROWN);
    DrawCircle(95, 85, 30.0f, GOLD);
    DrawCircle(95, 85, 24.0f, YELLOW);
    DrawText(TextFormat("%d", mSunCount), 145, 68, 36, YELLOW);
}

} // namespace PvZ
