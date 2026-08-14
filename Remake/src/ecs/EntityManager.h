#pragma once
#include "Entity.h"
#include <vector>
#include <memory>

namespace PvZ {

class EntityManager {
public:
    static EntityManager& Instance();

    std::shared_ptr<Entity> CreateEntity(EntityTag tag = EntityTag::NONE);
    void Update();
    void Clear();

    const std::vector<std::shared_ptr<Entity>>& GetEntities() const { return mEntities; }
    std::vector<std::shared_ptr<Entity>> GetEntitiesByTag(EntityTag tag) const;

private:
    EntityManager() = default;
    ~EntityManager() = default;

    EntityID mNextID = 1;
    std::vector<std::shared_ptr<Entity>> mEntities;
};

} // namespace PvZ
