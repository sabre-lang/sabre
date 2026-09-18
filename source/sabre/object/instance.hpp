#ifndef _SABRE_OBJECT_INSTANCE_HPP
#define _SABRE_OBJECT_INSTANCE_HPP

/// Sabre Includes
#include "sabre/object/class.hpp"

namespace Sabre {

/// @brief Instance Attributes.
template <> struct Object::Wrapper<Object::Instance> {
  //  PROPERTIES  //

  /// @brief Bound prototype instance.
  Class prototype;

  /// @brief Bound instance fields.
  Fields fields = {};

  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a defaulted object instance.
   * @param isolate               Runtime isolate.
   */
  explicit Wrapper(Runtime::Isolate *isolate);

  /**
   * @brief Constructs a class instance.
   * @param isolate               Runtime isolate.
   * @param inherits              Class to inherit.
   */
  explicit Wrapper(const Class &inherits) : prototype(inherits) {}
};

/// @brief Instance Interface.
struct Object::Instance : public Object::Mixin<Object::Instance> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

  //  PUBLIC METHODS  //

  inline constexpr Fields &fields() const noexcept { return m_wrapper()->fields; }
  inline constexpr const Class &prototype() const noexcept { return m_wrapper()->prototype; }
  inline constexpr const String::Any &name() const noexcept { return prototype().name(); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handlers yielding instances safely.
   * @param self                          Value instance.
   * @param yield                         Yield callback.
   */
  static void m_yield(const Instance &self, Globals::Each &yield);

  /**
   * @brief Handles printing values.
   * @param os                    Output stream.
   * @param self                  Object instance.
   */
  static inline void m_print(std::ostream &os, const Instance &self) {
    os << $::Dye::cyan("<{0}: 0x{1:08X}>", self.name(), self.m_pointer.value());
  }
};

} // namespace Sabre

#endif
