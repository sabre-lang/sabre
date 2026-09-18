#ifndef _SABRE_TYPE_INSTANCE_HPP
#define _SABRE_TYPE_INSTANCE_HPP

/// Type Includes
#include "sabre/type/compound/prototype.hpp"

namespace Sabre::Type {

/// @brief Instance Typing.
class Instance : public Mixin<Instance> {
  //  PROPERTIES  //

  /// @brief Associated class typing.
  $::Shared::Pointer<Prototype> m_prototype = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a dynamic object instance.
  explicit Instance() = default;

  /**
   * @brief Constructs an instance-type.
   * @param prototype             Class prototype.
   */
  explicit Instance(const $::Shared::Pointer<Prototype> &prototype) : m_prototype(prototype) {}

  //  PROPERTIES  //

  /// @brief Gets the underlying prototype.
  inline constexpr const $::Shared::Pointer<Prototype> &prototype() const noexcept { return m_prototype; }

  /// @brief Gets the associated shape for this instance.
  inline constexpr Shape::Underlying shape() const noexcept final {
    return m_prototype ? m_prototype->m_shape : Shape::Lookup<Object::Instance>();
  }

  /// @brief Gets the associated truthiness.
  inline constexpr $::Unit::Ternary truthiness() const noexcept final {
    // ignore if there is no underlying truthiness
    if (m_prototype == nullptr) return true;

    // cast based on the incoming details
    switch (m_prototype->m_base()) {
    case Shape::Lookup<String::Any>(): $_FALLTHROUGH;
    case Shape::Lookup<Number::Tagged>(): $_FALLTHROUGH;
    case Shape::Lookup<Value::Boolean>(): return $::Unit::Unknown();
    default: return true; // all other object types will be truthy
    }
  }

  /**
   * @brief Handles looking up fields.
   * @param field                 Field to lookup.
   */
  Entity lookup(const $::String::View &field) const final;

  /**
   * @brief Handles transforming the type.
   * @param kind                  Operator kind.
   */
  Erased apply(Operator::Kind kind) const final;
  Erased apply(Operator::Kind kind, const Erased &right) const final;

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles inferring class types.
   * @param constraints               Generic constraints.
   */
  Erased m_infer(Constraints *constraints) const final;

  /**
   * @brief Handles running a unification pass.
   * @param candidate                 Candidate to unify.
   * @param constraints               Generic parameter constraints.
   */
  bool m_unify(const Erased &candidate, Constraints *constraints) const final;

  /**
   * @brief Handles printing the type.
   * @param os                        Output stream.
   * @param self                      Instance value.
   */
  static void m_print(std::ostream &os, const Instance &self);
};

} // namespace Sabre::Type

#endif
