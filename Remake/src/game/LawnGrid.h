#pragma once
#include "raylib.h"
#include "../ecs/Entity.h"
#include <memory>
#include <array>

namespace PvZ {

class LawnGrid {
public:
    static constexpr int COLS = 9;
    static constexpr int ROWS = 5;

    LawnGrid(float startX = 350.0f, float startY = 180.0f, float cellWidth = 140.0f, float cellHeight = 160.0f);

    Vector2 GridToWorld(int col, int row) const;
    bool WorldToGrid(Vector2 worldPos, int& outCol, int& outRow) const;

    bool CanPlantAt(int col, int row) const;
    void PlacePlant(int col, int row, std::shared_ptr<Entity> plant);
    void RemovePlant(int col, int row);

    void RenderGrid(Vector2 virtualMousePos) const;

private:
    float mStartX;
    float mStartY;
    float mCellWidth;
    float mCellHeight;
    std::array<std::array<std::shared_ptr<Entity>, ROWS>, COLS> mGrid;
};

} // namespace PvZ
