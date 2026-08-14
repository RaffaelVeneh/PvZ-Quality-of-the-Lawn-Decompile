#include "engine/Viewport.h"
#include "engine/AssetManager.h"
#include "ecs/EntityManager.h"
#include "game/LawnGrid.h"
#include "game/SunManager.h"
#include <iostream>

int main() {
    PvZ::Viewport viewport(1280, 720);
    viewport.InitWindow("Plants vs. Zombies - Modern Remake Engine");

    // Load Data-Driven Configs
    PvZ::AssetManager::Instance().LoadJson("plants", "assets/data/plants.json");
    PvZ::AssetManager::Instance().LoadJson("zombies", "assets/data/zombies.json");

    PvZ::LawnGrid grid(350.0f, 180.0f, 140.0f, 160.0f);
    PvZ::SunManager::Instance().Init();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        viewport.Update();

        Vector2 mouseVirtual = viewport.GetVirtualMousePosition();
        bool mouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        // Update Managers
        PvZ::SunManager::Instance().Update(dt, mouseVirtual, mouseClicked);
        PvZ::EntityManager::Instance().Update();

        viewport.BeginDrawing();
            // Background
            DrawRectangle(0, 0, (int)PvZ::Viewport::VIRTUAL_WIDTH, (int)PvZ::Viewport::VIRTUAL_HEIGHT, DARKGREEN);

            // Render 9x5 Lawn Grid & Cell Highlight
            grid.RenderGrid(mouseVirtual);

            // Render Sun Currency & Sun Entities
            PvZ::SunManager::Instance().Render();

            // Status Info HUD
            DrawText("PvZ Modern Remake Engine - Phase 1 Foundation", 50, 1020, 24, RAYWHITE);
            DrawText(TextFormat("Virtual Mouse: (%d, %d)", (int)mouseVirtual.x, (int)mouseVirtual.y), 1500, 1020, 24, YELLOW);

        viewport.EndDrawing();
    }

    return 0;
}
