#include "LawnGrid.h"

namespace PvZ {

LawnGrid::LawnGrid(float startX, float startY, float cellWidth, float cellHeight)
    : mStartX(startX), mStartY(startY), mCellWidth(cellWidth), mCellHeight(cellHeight) {
    for (int col = 0; col < COLS; col++) {
        for (int row = 0; row < ROWS; row++) {
            mGrid[col][row] = nullptr;
        }
    }
}

Vector2 LawnGrid::GridToWorld(int col, int row) const {
    float x = mStartX + col * mCellWidth + mCellWidth * 0.5f;
    float y = mStartY + row * mCellHeight + mCellHeight * 0.5f;
    return { x, y };
}

bool LawnGrid::WorldToGrid(Vector2 worldPos, int& outCol, int& outRow) const {
    if (worldPos.x < mStartX || worldPos.x >= mStartX + COLS * mCellWidth ||
        worldPos.y < mStartY || worldPos.y >= mStartY + ROWS * mCellHeight) {
        return false;
    }

    outCol = (int)((worldPos.x - mStartX) / mCellWidth);
    outRow = (int)((worldPos.y - mStartY) / mCellHeight);
    return true;
}

bool LawnGrid::CanPlantAt(int col, int row) const {
    if (col < 0 || col >= COLS || row < 0 || row >= ROWS) return false;
    return mGrid[col][row] == nullptr;
}

void LawnGrid::PlacePlant(int col, int row, std::shared_ptr<Entity> plant) {
    if (col >= 0 && col < COLS && row >= 0 && row < ROWS) {
        mGrid[col][row] = plant;
    }
}

void LawnGrid::RemovePlant(int col, int row) {
    if (col >= 0 && col < COLS && row >= 0 && row < ROWS) {
        if (mGrid[col][row]) {
            mGrid[col][row]->Destroy();
            mGrid[col][row] = nullptr;
        }
    }
}

void LawnGrid::RenderGrid(Vector2 virtualMousePos) const {
    for (int col = 0; col < COLS; col++) {
        for (int row = 0; row < ROWS; row++) {
            Color gridColor = ((col + row) % 2 == 0) ? LIME : GREEN;
            DrawRectangleRec({ mStartX + col * mCellWidth, mStartY + row * mCellHeight, mCellWidth - 4, mCellHeight - 4 }, gridColor);
        }
    }

    // Highlight hovered cell
    int mouseCol, mouseRow;
    if (WorldToGrid(virtualMousePos, mouseCol, mouseRow)) {
        DrawRectangleLinesEx({ mStartX + mouseCol * mCellWidth, mStartY + mouseRow * mCellHeight, mCellWidth - 4, mCellHeight - 4 }, 4.0f, YELLOW);
    }
}

} // namespace PvZ
