#include "ember/sys/collision/SpatialPartition.hpp"
#include "ember/sys/collision/CollisionEngine.hpp"
#include "ember/core/Scene.hpp"

ember::Scene& ember::sys::collision::SpatialPartition::scene() { return _owning_engine->scene(); }
