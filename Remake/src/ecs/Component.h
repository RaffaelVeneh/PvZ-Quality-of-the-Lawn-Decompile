#pragma once
#include "raylib.h"
#include <string>

namespace PvZ {

struct PositionComponent {
    float x = 0.0f;
    float y = 0.0f;
};

struct VelocityComponent {
    float vx = 0.0f;
    float vy = 0.0f;
};

struct HealthComponent {
    int hp = 100;
    int maxHp = 100;
};

struct RenderComponent {
    Texture2D texture = { 0 };
    Rectangle srcRect = { 0, 0, 0, 0 };
    Vector2 origin = { 0, 0 };
    Color tint = WHITE;
    float scale = 1.0f;
};

struct PlantComponent {
    std::string id;
    int gridCol = 0;
    int gridRow = 0;
    float actionTimer = 0.0f;
    float actionInterval = 1.4f;
};

struct ZombieComponent {
    std::string id;
    int laneRow = 0;
    float eatTimer = 0.0f;
    bool isEating = false;
};

struct ProjectileComponent {
    float damage = 20.0f;
    int laneRow = 0;
    bool isSlow = false;
};

} // namespace PvZ
