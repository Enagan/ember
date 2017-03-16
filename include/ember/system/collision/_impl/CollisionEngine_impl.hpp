namespace ember {
namespace system {
namespace collision {

template <typename SpatialPartitionSubType, typename... Args>
void CollisionEngine::withSpatialPartitioner(Args&&... args) {
    _spatial_partitioner = std::unique_ptr<SpatialPartition>(new SpatialPartitionSubType(std::forward<Args>(args)...));
    _spatial_partitioner->_owning_engine = this;
}

template <typename SpatialPartitionSubType>
void CollisionEngine::hasSpatialPartitionerOfType() {
    return dynamic_cast<SpatialPartitionSubType*>(_spatial_partitioner.get()) != nullptr;
}

template <typename SpatialPartitionSubType>
SpatialPartitionSubType& CollisionEngine::refSpatialPartitionerOfType() {
    return *(dynamic_cast<SpatialPartitionSubType*>(_spatial_partitioner.get()));
}

}
}
}
