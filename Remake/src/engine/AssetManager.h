#pragma once
#include "raylib.h"
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

namespace PvZ {

class AssetManager {
public:
    static AssetManager& Instance();

    bool LoadTexture(const std::string& id, const std::string& filepath);
    Texture2D GetTexture(const std::string& id) const;
    void UnloadTexture(const std::string& id);

    bool LoadJson(const std::string& id, const std::string& filepath);
    nlohmann::json GetJson(const std::string& id) const;

    void Clear();

private:
    AssetManager() = default;
    ~AssetManager();

    std::unordered_map<std::string, Texture2D> mTextures;
    std::unordered_map<std::string, nlohmann::json> mJsonData;
};

} // namespace PvZ
