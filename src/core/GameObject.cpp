#include "ember/core/GameObject.hpp"
#include "ember/core/Scene.hpp"
#include "ember/core/Behaviour.hpp"

using namespace ember;

GameObject::GameObject() {}

GameObject::GameObject(GameObject&& other) : _hasStarted(other._hasStarted){
	_behaviours.swap(other._behaviours);
	for (auto& behaviour : _behaviours) {
		behaviour.second->setGameObjectOwner(this);
	}
}

GameObject::~GameObject() {
    // Clear components gameobject pointer so they can deal with un-atachment
    for (auto& behaviour : _behaviours) {
        behaviour.second->onEnd();
		behaviour.second->setGameObjectOwner(nullptr);
	}
}

GameObject& GameObject::operator=(GameObject&& other) {
	_behaviours.swap(other._behaviours);
	for (auto& behaviour : _behaviours) {
		behaviour.second->setGameObjectOwner(this);
	}
	return *this;
}

void GameObject::onStart() {
	_hasStarted = true;
	for (auto& behaviour : _behaviours) {
		behaviour.second->onStart();
	}
}
void GameObject::onUpdate(double deltaT) {
	for (auto& behaviour : _behaviours) {
		behaviour.second->onUpdate(deltaT);
	}
}
void GameObject::onPostUpdate() {
	for (auto& behaviour : _behaviours) {
		behaviour.second->onPostUpdate();
	}
}
void GameObject::onPostCollision() {
	for (auto& behaviour : _behaviours) {
		behaviour.second->onPostCollision();
	}
}

void GameObject::Destroy() {
    scene().removeGameObject(object_id());
}