#ifndef Ember_Serializable_hpp
#define Ember_Serializable_hpp

namespace ember {
namespace addons {


/// Serializable Into allows your Behaviours to interface with the ember Serialization facilities (accessible through the GameObject API)
/// By making your custom Behaviour class inherit from SerializableInto, you state that your behaviour can be serialized into an object of the type
/// TypeToSerializeTo.
template <typename TypeToSerializeTo>
class SerializableInto {
public:

    /// Must be overriten. Should make a full serialization of your Behaviour into the object 'into'.
    /// Between update cycles, it's guaranteed this callback will only be called once, regardless of how many times you attempt to Serialize a GameObject.
    virtual void SerializeInto(TypeToSerializeTo& into) = 0;

    /// Should be overwritten, if your behaviour is changeable.
    /// The Serialization in this step must only contain the changes since the previous call to the partial serialization of the full Serialization above.
    /// It must return false, if no serialization took place.
    /// Between update cycles, it's guaranteed this callback will only be called once, regardless of how many times you attempt to Serialize a GameObject.
    virtual bool PartialSerializeInto(TypeToSerializeTo& /*into*/){ return false; }

    /// Should be overwritten for greater performance, will inform the gameobject if the object is dirty for serializations of this type.
    /// It can prevent useless calls to PartialSerializeInto, if nothing changed.
    /// The currently cached serialization object is passed into the function in case it's helpfull to determine if anything has changed.
    virtual bool IsSerializationDirty(const std::shared_ptr<TypeToSerializeTo>& /*currently_cached*/ = nullptr) { return true; }
};

}}
#endif
