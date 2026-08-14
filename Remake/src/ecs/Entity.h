#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace PvZ {

using EntityID = unsigned int;

enum class EntityTag {
    NONE,
    PLANT,
    ZOMBIE,
    PROJECTILE,
    SUN,
    UI
};

class Entity {
public:
    Entity(EntityID id, EntityTag tag = EntityTag::NONE) : mID(id), mTag(tag), mActive(true) {}

    EntityID GetID() const { return mID; }
    EntityTag GetTag() const { return mTag; }
    bool IsActive() const { return mActive; }
    void Destroy() { mActive = false; }

    template<typename T, typename... Args>
    T& AddComponent(Args&&... args) {
        auto comp = std::make_shared<T>(std::forward<Args>(args)...);
        mComponents[typeid(T)] = comp;
        return *comp;
    }

    template<typename T>
    T* GetComponent() {
        auto it = mComponents.find(typeid(T));
        if (it != mComponents.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    template<typename T>
    bool HasComponent() const {
        return mComponents.find(typeid(T)) != mComponents.end();
    }

private:
    EntityID mID;
    EntityTag mTag;
    bool mActive;
    std::unordered_map<std::type_index, std::shared_ptr<void>> mComponents;
};

} // namespace PvZ
