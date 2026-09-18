#ifndef _SABRE_OBJECT_CLASS_HPP
#define _SABRE_OBJECT_CLASS_HPP

/// Sabre Includes
#include "sabre/member/reference.hpp"
#include "sabre/object/mixin.hpp"
#include "sabre/string/common.hpp"

/// Forward Definitions
$_FWD(Sabre::Object, using Fields = $::Map::Base<Value::Symbol, $::Unique::Pointer<Member::Reference>>)

namespace Sabre {

/// @brief Class Attributes.
template <> struct Object::Wrapper<Object::Class> {
  //  PROPERTIES  //

  Fields statics;          // Static fields.
  Value::Any parent;       // Ancestor value.
  String::Any name;        // Exposed class name.
  Shape::Underlying shape; // Bound shape value.

  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a class prototype.
   * @param name                  Name of class.
   * @param shape                 Shape of class.
   * @param parent                Ancestor class.
   */
  explicit Wrapper(const String::Any &name, Shape::Underlying shape, const Value::Any &parent = {}) :
      parent(parent), name(name), shape(shape) {}

  /**
   * @brief Constructs a class prototype.
   * @param isolate               Runtime isolate.
   * @param name                  Name of class.
   */
  explicit Wrapper(Runtime::Isolate *isolate, const $::String::View &name, Shape::Underlying shape) :
      Wrapper(isolate, name, shape, {}) {}

  /**
   * @brief Constructs a class prototype.
   * @param isolate               Runtime isolate.
   * @param name                  Name of class.
   * @param parent                Ancestor class.
   */
  explicit Wrapper(
      Runtime::Isolate *isolate, const $::String::View &name, Shape::Underlying shape, const Value::Any &parent
  ) : Wrapper(String::Any(isolate, name), shape, parent) {}
};

/// @brief Class Interface.
struct Object::Class : public Object::Mixin<Object::Class> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

  //  PUBLIC METHODS  /

  inline constexpr Fields &statics() const noexcept { return m_wrapper()->statics; }
  inline constexpr Value::Any &parent() const noexcept { return m_wrapper()->parent; }
  inline constexpr String::Any &name() const noexcept { return m_wrapper()->name; }
  inline constexpr Shape::Underlying shape() const noexcept { return m_wrapper()->shape; }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handlers yielding classes safely.
   * @param self                          Class instance.
   * @param yield                         Yield callback.
   */
  static void m_yield(const Class &self, Globals::Each &yield);

  /**
   * @brief Handles printing values.
   * @param os                    Output stream.
   * @param self                  Class instance.
   */
  static inline void m_print(std::ostream &os, const Class &self) {
    os << $::Dye::cyan("<{0}: {1}>", self.brand(), self.name());
  }
};

} // namespace Sabre

#endif
