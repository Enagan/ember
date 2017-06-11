#ifndef Ember_DirtyValue_hpp
#define Ember_DirtyValue_hpp

namespace ember {
namespace addons {

/// Dirty Value is an utility class that can be used in Behaviours to encapsulate class variables of any type.
/// The encapsulated value will always be accessible through get(), however a dirty flag will be set to true, every time the
/// value is set(). This flag can be checked, as well as cleaned, if the behaviour acknowledges the change in value.
template <typename ParamType>
class DirtyValue {
public:
    DirtyValue(ParamType data) : _data(std::move(data)), _is_dirty(true) {}

    const ParamType& get() const { return _data; }
    void set(ParamType new_data) { std::swap(_data, new_data); _is_dirty = true; }

    bool is_dirty() const { return _is_dirty; }
    void clean() { _is_dirty = false; }

private:
    bool _is_dirty;
    ParamType _data;
};

}}
#endif
