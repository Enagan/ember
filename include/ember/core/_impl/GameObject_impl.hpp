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
	return *this;
}

template <typename BehaviourSubType>
bool GameObject::hasBehaviour() {
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