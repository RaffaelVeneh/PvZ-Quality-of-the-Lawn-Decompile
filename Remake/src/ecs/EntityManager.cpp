#include "EntityManager.h"
#include <algorithm>

namespace PvZ {

EntityManager& EntityManager::Instance() {
    static EntityManager instance;
    return instance;
}

std::shared_ptr<Entity> EntityManager::CreateEntity(EntityTag tag) {
    auto entity = std::make_shared<Entity>(mNextID++, tag);
    mEntities.push_back(entity);
    return entity;
}

void EntityManager::Update() {
    // Remove inactive entities
    mEntities.erase(
        std::remove_if(mEntities.begin(), mEntities.end(),
            [](const std::shared_ptr<Entity>& entity) { return !entity->IsActive(); }),
        mEntities.end()
    );
}

std::vector<std::shared_ptr<Entity>> EntityManager::GetEntitiesByTag(EntityTag tag) const {
    std::vector<std::shared_ptr<Entity>> result;
    for (const auto& entity : mEntities) {
        if (entity->IsActive() && entity->GetTag() == tag) {
            result.push_back(entity);
        }
    }
    return result;
}

void EntityManager::Clear() {
    mEntities.clear();
    mNextID = 1;
}

} // namespace PvZ
