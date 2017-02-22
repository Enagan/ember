#include "ember/core/GameObject.hpp"

namespace ember {

template <typename BehaviourSubType>
bool Behaviour::hasSibling() {
    return is_attached() && game_object().hasBehaviour<BehaviourSubType>();
}

template <typename BehaviourSubType>
BehaviourSubType& Behaviour::refSibling() throw(std::invalid_argument) {
    if (!hasSibling<BehaviourSubType>()) {
        throw std::invalid_argument("Behaviour::refSibling - Behaviour " + std::string(typeid(BehaviourSubType).name()) + " not present in parent GameObject");
    }
    return game_object().refBehaviour<BehaviourSubType>();
}

template <typename BehaviourSubType>
const BehaviourSubType& Behaviour::refSibling() const throw(std::invalid_argument) {
    if (!hasSibling<BehaviourSubType>()) {
        throw std::invalid_argument("Behaviour::refSibling - Behaviour " + std::string(typeid(BehaviourSubType).name()) + " not present in parent GameObject");
    }
    return game_object().refBehaviour<BehaviourSubType>();
}

template <typename BehaviourSubType>
std::weak_ptr<BehaviourSubType> Behaviour::getSibling() {
    if (hasSibling<BehaviourSubType>()) {
        return game_object().getBehaviour<BehaviourSubType>();
    }
    return std::weak_ptr<BehaviourSubType>();
}

}
