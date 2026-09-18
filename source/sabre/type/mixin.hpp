#ifndef _SABRE_TYPE_MIXIN_HPP
#define _SABRE_TYPE_MIXIN_HPP

/// Sabre Includes
#include "sabre/forward/type.hpp"
#include "sabre/operator/kind.hpp"
#include "sabre/shape/lookup.hpp"

namespace Sabre::Type {

/// @brief Allows Defining Specialized Types.
template <class T, std::derived_from<Unit> B = Unit> using Mixin = $::RTTI::Mixin<T, B>;

/// @brief Encapsulates Type Units.
struct $_ABSTRACT Unit : public $::Shared::Recast<Unit>, public $::RTTI::Dynamic {
  //  TYPEDEFS  //

  /// @brief Allow transformers internal access.
  friend class Transform;

  //  CONSTRUCTORS  //

  /// @brief Virtual abstract destructor.
  virtual ~Unit() = default;

  //  PUBLIC METHODS  //

  /// @brief Gets the internal value shape of the type.
  virtual inline Shape::Underlying shape() const noexcept { return Shape::Invalid(); }

  /// @brief Gets a types truthiness (any of three states).
  virtual inline $::Unit::Ternary truthiness() const noexcept { return $::Unit::Unknown(); }

  /// @brief Allows casting to derived types.
  template <std::derived_from<Unit> T> inline constexpr $::Shared::Pointer<T> as() noexcept {
    return $::RTTI::Assert<T>(this), m_as<T>();
  }

  /// @brief Determines if a type matches a specific typing.
  template <std::derived_from<Unit>... Ts> inline constexpr bool is() const noexcept { return Dynamic::is<Ts...>(); }
  template <std::derived_from<Value::Any>... Ts> inline constexpr bool is() const noexcept {
    return ((Shape::Lookup<Ts>() == shape()) || ...);
  }

  /**
   * @brief Searches for a field on a type.
   * @param field                       Field to get.
   */
  virtual Entity lookup(const $::String::View &field) const;

  /**
   * @brief Handles applying unary/binary operations to a type.
   * @param kind                        Kind of operator.
   * @param right                       Optional RHS type.
   */
  virtual Erased apply(Operator::Kind kind) const;
  virtual Erased apply(Operator::Kind kind, const Erased &right) const;

  /**
   * @brief Handles inferring generic types.
   * @param constraints                 Generic constraints.
   */
  Erased infer(Constraints *constraints = nullptr) const;

  /**
   * @brief Checks if a candidate is a subtype of this type.
   * @param candidate                   Candidate type.
   * @param constraints                 Generic constraints.
   */
  bool unify(const Erased &candidate, Constraints *constraints = nullptr) const;

protected:
  //  PROTECTED METHODS  //

  /**
   * @brief Handles inferring generic types.
   * @param constraints                 Generic constraints.
   */
  virtual Erased m_infer(Constraints *constraints = nullptr) const;

  /**
   * @brief Handles solving subtype checking for various items.
   * @param candidate                   Candidate type.
   * @param constraints                 Generic constraints.
   */
  bool m_solve(const Generic *generic, Constraints *constraints) const;
  bool m_solve(const Parameter *parameter, Constraints *constraints) const;

  /**
   * @brief The core subtype checker for type units.
   * @param candidate                   Candidate type.
   * @param constraints                 Generic constraints.
   */
  virtual bool m_unify(const Erased &candidate, Constraints *constraints = nullptr) const = 0;

  /**
   * @brief Handles printing type units.
   * @param os                          Output stream.
   * @param self                        Type instance.
   */
  static void m_print(std::ostream &os, const Unit &self);
};

} // namespace Sabre::Type

#endif
