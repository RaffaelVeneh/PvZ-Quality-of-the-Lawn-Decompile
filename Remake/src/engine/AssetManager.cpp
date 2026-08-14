#include "AssetManager.h"
#include <fstream>
#include <iostream>

namespace PvZ {

AssetManager& AssetManager::Instance() {
    static AssetManager instance;
    return instance;
}

AssetManager::~AssetManager() {
    Clear();
}

bool AssetManager::LoadTexture(const std::string& id, const std::string& filepath) {
    if (mTextures.find(id) != mTextures.end()) {
        return true;
    }

    Texture2D tex = ::LoadTexture(filepath.c_str());
    if (tex.id == 0) {
        std::cerr << "[AssetManager] Failed to load texture: " << filepath << std::endl;
        return false;
    }

    mTextures[id] = tex;
    return true;
}

Texture2D AssetManager::GetTexture(const std::string& id) const {
    auto it = mTextures.find(id);
    if (it != mTextures.end()) {
        return it->second;
    }
    return Texture2D{ 0 };
}

void AssetManager::UnloadTexture(const std::string& id) {
    auto it = mTextures.find(id);
    if (it != mTextures.end()) {
        ::UnloadTexture(it->second);
        mTextures.erase(it);
    }
}

bool AssetManager::LoadJson(const std::string& id, const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[AssetManager] Failed to open JSON file: " << filepath << std::endl;
        return false;
    }

    nlohmann::json data;
    file >> data;
    mJsonData[id] = data;
    return true;
}

nlohmann::json AssetManager::GetJson(const std::string& id) const {
    auto it = mJsonData.find(id);
    if (it != mJsonData.end()) {
        return it->second;
    }
    return nlohmann::json();
}

void AssetManager::Clear() {
    for (auto& pair : mTextures) {
        ::UnloadTexture(pair.second);
    }
    mTextures.clear();
    mJsonData.clear();
}

} // namespace PvZ
