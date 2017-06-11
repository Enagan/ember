#ifndef Ember_ListensTo_hpp
#define Ember_ListensTo_hpp

namespace ember {
namespace addons {

/// Listens to is used to interface with the event system present in ember.
/// By making your custom Behaviour class inherit from ListenTo, you can be ready to handle events of TypeToReceive.
template <typename TypeToReceive>
class ListensTo {
public:
    virtual void Handle(const TypeToReceive& message) = 0;
};

}}
#endif
