#ifndef _SABRE_MEMBER_PROPERTY_HPP
#define _SABRE_MEMBER_PROPERTY_HPP

/// Sabre Includes
#include "sabre/member/descriptor.hpp"

namespace Sabre::Member {

/// @brief Getter Typing.
using Getter = $::Unique::Functor<Value::Any(Runtime::Isolate *, const Value::Any &) const>;

/// @brief Setting Typing.
using Setter = $::Unique::Functor<Value::Any(Runtime::Isolate *, const Value::Any &, const Value::Any &)>;

/// @brief Property Member Descriptor.
class Property : public Descriptor {
  //  PROPERTIES  //

  Getter m_getter = nullptr; // Getter function.
  Setter m_setter = nullptr; // Setter function.

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a property descriptor.
  explicit Property() : Descriptor(true) {}
  explicit Property(Getter &&getter) : Descriptor(true), m_getter(std::move(getter)) {}
  explicit Property(Setter &&setter) : Descriptor(false), m_setter(std::move(setter)) {}
  explicit Property(Getter &&getter, Setter &&setter) :
      Descriptor(false), m_getter(std::move(getter)), m_setter(std::move(setter)) {}

  //  PUBLIC METHODS  //

  /**
   * @brief Handles getting the value.
   * @param isolate           Runtime isolate.
   * @param self              Self value.
   */
  Value::Any getter(Runtime::Isolate *isolate, const Value::Any &self) const final;

  /**
   * @brief Handles setting the value.
   * @param isolate           Runtime isolate.
   * @param self              Self value.
   * @param value             Value to assign.
   */
  Value::Any setter(Runtime::Isolate *isolate, const Value::Any &self, const Value::Any &value) final;
};

} // namespace Sabre::Member

#endif
