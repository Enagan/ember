#include "ember/core/System.hpp"

namespace ember {

void BaseSystem::FilterGameObject(const std::shared_ptr<GameObject>& shared_obj) {
    if (_filter_fun(*shared_obj)) {
        if (_managed_objects.count(shared_obj->object_id()) == 0) {
            onGameObjectAdded(shared_obj);
        }
    } else {
        onGameObjectRemoved(shared_obj);
    }
}

void BaseSystem::onGameObjectAdded(const std::weak_ptr<GameObject>& weak_obj) {
    if (auto shared_obj = weak_obj.lock()) {
        _managed_objects[shared_obj->object_id()] = weak_obj;
    }
}

void BaseSystem::onGameObjectRemoved(const std::weak_ptr<GameObject>& weak_obj) {
    if (auto shared_obj = weak_obj.lock()) {
        _managed_objects.erase(shared_obj->object_id());
    }
}

void BaseSystem::onPreUpdate() {
    for (auto iter = _managed_objects.begin(); iter != _managed_objects.end(); ) {
        if (auto shared_obj = iter->second.lock()) {
            onPreUpdate(*shared_obj);
            iter++;
        } else {
            auto to_erase = iter;
            iter++;
            _managed_objects.erase(to_erase);
        }
    }
}

void BaseSystem::onUpdate(double deltaT) {
    for (auto iter = _managed_objects.begin(); iter != _managed_objects.end();) {
        if (auto shared_obj = iter->second.lock()) {
            onUpdate(deltaT, *shared_obj);
            iter++;
        } else {
            auto to_erase = iter;
            iter++;
            _managed_objects.erase(to_erase);
        }
    }
}

void BaseSystem::onPostUpdate() {
    for (auto iter = _managed_objects.begin(); iter != _managed_objects.end();) {
        if (auto shared_obj = iter->second.lock()) {
            onPostUpdate(*shared_obj);
            iter++;
        } else {
            auto to_erase = iter;
            iter++;
            _managed_objects.erase(to_erase);
        }
    }
}

}
