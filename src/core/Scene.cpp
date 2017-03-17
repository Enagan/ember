#include <iostream>
#include "ember/core/Scene.hpp"

using namespace ember;

Scene::Scene() {};

Scene::~Scene() {};

Scene::Scene(Scene&& other) {
    Swap(std::move(other));
}

Scene& Scene::operator=(Scene&& other) {
	Swap(std::move(other));
	return *this;
}

void Scene::onStart() {
	_hasStarted = true;
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> delayed_deletion_copy(_objects_in_scene);
	for (auto& game_object : delayed_deletion_copy) {
		game_object.second->onStart();
	}
    for (auto& system_in_scene : _systems_in_scene) {
        system_in_scene.second->onStart();
    }
}

void Scene::onPreUpdate() {
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> delayed_deletion_copy(_objects_in_scene);
	for (auto& game_object : delayed_deletion_copy) {
		game_object.second->onPreUpdate();
        if(game_object.second->_behaviours_changed) {
            FilterGameObjectThroughAllSystems(game_object.second);
        }
	}
    for (auto& system_in_scene : _systems_in_scene) {
        system_in_scene.second->onPreUpdate();
    }
}

void Scene::onUpdate(double deltaT) {
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> delayed_deletion_copy(_objects_in_scene);
	for (auto& game_object : delayed_deletion_copy) {
		game_object.second->onUpdate(deltaT);
	}
    for (auto& system_in_scene : _systems_in_scene) {
        system_in_scene.second->onUpdate(deltaT);
    }
}

void Scene::onPostUpdate() {
    std::unordered_map<GameObject::id, std::shared_ptr<GameObject>> delayed_deletion_copy(_objects_in_scene);
	for (auto& game_object : delayed_deletion_copy) {
		game_object.second->onPostUpdate();
	}
    for (auto& system_in_scene : _systems_in_scene) {
        system_in_scene.second->onPostUpdate();
    }
}

GameObject& Scene::addGameObject() {
    auto id_for_gameobject = _next_game_object_index++;
    _objects_in_scene[id_for_gameobject] = std::make_shared<GameObject>();
    auto& new_object = _objects_in_scene[id_for_gameobject];
    new_object->_id = id_for_gameobject;
    new_object->_parent_scene = this;
	if (_hasStarted) {
		new_object->onStart();
	}
	return *new_object;
}

bool Scene::hasGameObject(GameObject::id index) const {
    return _objects_in_scene.count(index) == 1;
}

GameObject& Scene::getGameObject(GameObject::id index) {
    return *_objects_in_scene.at(index);
}

void Scene::removeGameObject(GameObject::id index) {
    _objects_in_scene.erase(index);
}

void Scene::FilterGameObjectThroughAllSystems(const std::shared_ptr<GameObject>& object) {
    for (auto& system_in_scene : _systems_in_scene) {
        system_in_scene.second->FilterGameObject(object);
    }
}

void Scene::FilterAllGameObjectsThroughSystem(const std::shared_ptr<BaseSystem>& system_to_filter) {
    for (const auto& game_object : _objects_in_scene) {
        system_to_filter->FilterGameObject(game_object.second);
    }
}

void Scene::Swap(Scene&& other) {
    _next_game_object_index = other._next_game_object_index;
	_hasStarted = other._hasStarted;
    _objects_in_scene.swap(other._objects_in_scene);
    _systems_in_scene.swap(other._systems_in_scene);
    for (auto& object_in_scene : _objects_in_scene) {
        object_in_scene.second->_parent_scene = this;
    }
    for (auto& system_in_scene : _systems_in_scene) {
        system_in_scene.second->_parent_scene = this;
    }
}
