#include "ember/core/Behaviour.hpp"

namespace ember {
/// Implementation of template methods for Game Object
template <typename BehaviourSubType, typename... Args>
GameObject& GameObject::withBehaviour(Args&&... args) {
    auto type_index = std::type_index(typeid(BehaviourSubType));
    _behaviours.emplace(std::pair<std::type_index, std::shared_ptr<Behaviour>>{type_index, std::make_shared<BehaviourSubType>(std::forward<Args>(args)...)});
    _behaviours[type_index]->setGameObjectOwner(this);
    auto _id_for_behaviour = _next_behaviour_index++;
    _behaviours[type_index]->_id = Behaviour::id(_id, _id_for_behaviour);
	if (_hasStarted) {
		_behaviours[type_index]->onStart();
	}
    _behaviours_changed = true;
	return *this;
}

template <typename BehaviourSubType>
bool GameObject::hasBehaviour() const {
    return _behaviours.count(std::type_index(typeid(BehaviourSubType))) != 0;
}

template <typename BehaviourSubType>
BehaviourSubType& GameObject::refBehaviour() throw(std::invalid_argument) {
    if (_behaviours.count(std::type_index(typeid(BehaviourSubType))) == 0) {
        throw std::invalid_argument("GameObject::refBehaviour - Behaviour " + std::string(typeid(BehaviourSubType).name()) + " not present in GameObject");
    }
    return *(static_cast<BehaviourSubType*>(_behaviours[std::type_index(typeid(BehaviourSubType))].get()));
}

template <typename BehaviourSubType>
const BehaviourSubType& GameObject::refBehaviour() const throw(std::invalid_argument) {
    if (_behaviours.count(std::type_index(typeid(BehaviourSubType))) == 0) {
        throw std::invalid_argument("GameObject::refBehaviour - Behaviour " + std::string(typeid(BehaviourSubType).name()) + " not present in GameObject");
    }
    return *(static_cast<BehaviourSubType*>(_behaviours.at(std::type_index(typeid(BehaviourSubType))).get()));
}

template <typename BehaviourSubType>
std::weak_ptr<BehaviourSubType> GameObject::getBehaviour() {
    if (_behaviours.count(std::type_index(typeid(BehaviourSubType))) == 1) {
        return std::static_pointer_cast<BehaviourSubType>(_behaviours[std::type_index(typeid(BehaviourSubType))]);
    }
    return std::weak_ptr<BehaviourSubType>();
}

template <typename BehaviourType>
std::vector<std::weak_ptr<BehaviourType>> GameObject::getBehaviours() {
    std::vector<std::weak_ptr<BehaviourType>> return_vector;
    for (auto behaviour : _behaviours) {
        if(auto cast_component = std::dynamic_pointer_cast<BehaviourType>(behaviour.second))
        {
            return_vector.push_back(cast_component);
        }
    }
    return return_vector;
}

template <typename EventType>
void GameObject::CastEvent(const EventType& event) {
    for (auto behaviour : _behaviours) {
        if(auto cast_component = std::dynamic_pointer_cast<addons::ListensTo<EventType>>(behaviour.second))
        {
            cast_component->Handle(event);
        }
    }
}

struct GameObject::SerializedCacheBase {
    virtual ~SerializedCacheBase() = default;
    virtual bool IsCacheValid() = 0;
    virtual void ClearCachedData() = 0;
};

template <typename SerializableType>
struct GameObject::SerializedCacheSub : public GameObject::SerializedCacheBase {
    SerializedCacheSub(std::vector<std::weak_ptr<ember::addons::SerializableInto<SerializableType>>> responsible) :
    _cached_data(nullptr), _behaviours_responsible(std::move(responsible)) {}
    SerializedCacheSub(SerializableType to_cache,
        std::vector<std::weak_ptr<ember::addons::SerializableInto<SerializableType>>> responsible) :
    _cached_data(new SerializableType(std::move(to_cache))), _behaviours_responsible(std::move(responsible)) {}
    ~SerializedCacheSub() = default;
    bool IsCacheValid() override {
        for (auto serializable_behaviour : _behaviours_responsible) {
            if (serializable_behaviour.lock()->IsSerializationDirty(_cached_data)) {
                return false;
            }
        }
        return true;
    }
    void ClearCachedData() override
        { _cached_data = nullptr; }
    inline bool HasCachedData() const
        { return _cached_data != nullptr; }
    inline const SerializableType& GetCachedData() const
        { return *_cached_data; }
    inline const std::vector<std::weak_ptr<ember::addons::SerializableInto<SerializableType>>>& GetBehavioursResponsible() const
        { return _behaviours_responsible; }
    std::shared_ptr<SerializableType> _cached_data;
    std::vector<std::weak_ptr<ember::addons::SerializableInto<SerializableType>>> _behaviours_responsible;
};

template <typename SerializableInto>
bool GameObject::SerializeInto(SerializableInto& into) {
    bool serialized_something = false;
    auto type_index = std::type_index(typeid(SerializableInto));

    if (_serialization_cache.count(type_index) != 0) {
        SerializedCacheSub<SerializableInto>* cache =
            dynamic_cast<SerializedCacheSub<SerializableInto>*>(_serialization_cache[type_index].get());
        if (cache->HasCachedData()) {
            into = cache->GetCachedData();
            return true;
        } else {
            return false;
        }
    }

    auto serializable_behaviours = getBehaviours<ember::addons::SerializableInto<SerializableInto>>();
    for (auto serializable_component : serializable_behaviours) {
        serializable_component.lock()->SerializeInto(into);
        serialized_something = true;
    }
    if (serialized_something) {
        _serialization_cache.emplace(
            std::pair<std::type_index, std::unique_ptr<SerializedCacheBase>>
                {type_index,
                    std::unique_ptr<SerializedCacheBase>(new SerializedCacheSub<SerializableInto>(into, std::move(serializable_behaviours)))});
        return true;
    } else {
        _serialization_cache.emplace(
            std::pair<std::type_index, std::unique_ptr<SerializedCacheBase>>
                {type_index,
                    std::unique_ptr<SerializedCacheBase>(new SerializedCacheSub<SerializableInto>(std::move(serializable_behaviours)))});
        return false;
    }
}

template <typename SerializableInto>
bool GameObject::PartialSerializeInto(SerializableInto& into) {
    bool serialized_something = false;
    auto type_index = std::type_index(typeid(SerializableInto));

    if (_partial_serialization_cache.count(type_index) != 0) {
        SerializedCacheSub<SerializableInto>* cache =
            dynamic_cast<SerializedCacheSub<SerializableInto>*>(_partial_serialization_cache[type_index].get());
        if (cache->HasCachedData()) {
            into = cache->GetCachedData();
            return true;
        } else {
            return false;
        }
    }

    auto serializable_behaviours = getBehaviours<ember::addons::SerializableInto<SerializableInto>>();
    for (auto serializable_component : serializable_behaviours) {
        auto serialized_component = serializable_component.lock()->PartialSerializeInto(into);
        serialized_something = serialized_something || serialized_component;
    }
    if (serialized_something) {
        _partial_serialization_cache.emplace(
            std::pair<std::type_index, std::unique_ptr<SerializedCacheBase>>
                {type_index,
                    std::unique_ptr<SerializedCacheBase>(new SerializedCacheSub<SerializableInto>(into, std::move(serializable_behaviours)))});
        return true;
    } else {
        _partial_serialization_cache.emplace(
            std::pair<std::type_index, std::unique_ptr<SerializedCacheBase>>
                {type_index,
                    std::unique_ptr<SerializedCacheBase>(new SerializedCacheSub<SerializableInto>(std::move(serializable_behaviours)))});
        return false;
    }
}
}
