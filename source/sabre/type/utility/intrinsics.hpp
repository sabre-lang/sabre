#ifndef _SABRE_TYPE_INTRINSICS_HPP
#define _SABRE_TYPE_INTRINSICS_HPP

/// Sabre Includes
#include "sabre/type/mixin.hpp"
#include "sabre/value/void.hpp"

namespace Sabre::Type {

/// @brief Any Intrinsic Typing.
struct Any : public Mixin<Any> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

  //  PUBLIC METHODS  //

  /// @brief This will always resolve an entity.
  Entity lookup(const $::String::View &) const final;

  /// @brief Ensures all applications of operators return "Any" types.
  Erased apply(Operator::Kind) const final;
  Erased apply(Operator::Kind, const Erased &) const final;

protected:
  //  PRIVATE METHODS  //

  /// @brief Ensures all candidates resolve to "Any" types.
  inline bool m_unify(const Erased &, Constraints *) const final { return true; }

  /**
   * @brief Handles printing the type.
   * @param os                        Output stream.
   */
  static inline void m_print(std::ostream &os, const Any &) { os << "Any"; }
};

/// @brief Error Intrinsic Typing (for poisoning outputs).
struct Poison : public Mixin<Poison, Any> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles printing the type.
   * @param os                        Output stream.
   */
  static inline void m_print(std::ostream &os, const Poison &) { os << "Poison"; }
};

/// @brief Void Type Hint.
struct None : public Mixin<None> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

  //  PUBLIC METHODS  //

  /// @brief Gets the truthiness of the type hint.
  inline constexpr $::Unit::Ternary truthiness() const noexcept final { return false; }

  /// @brief Gets the internal shape of the type.
  inline Shape::Underlying shape() const noexcept final { return Shape::Lookup<Value::Void>(); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles unifying the type.
   * @param candidate                 Candidate to unify.
   */
  inline bool m_unify(const Erased &candidate, Constraints *) const final { return candidate->is<None>(); }

  /**
   * @brief Handles printing the type.
   * @param os                        Output stream.
   */
  static void m_print(std::ostream &os, const None &);
};

/// @brief Unset Intrinsic Typing.
struct Unset : public Mixin<Unset> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

  /// @brief This will always resolve an empty entity.
  Entity lookup(const $::String::View &) const final;

  /// @brief Ensures all applications of operators return "Unset" types.
  Erased apply(Operator::Kind) const final { return m_self(); }
  Erased apply(Operator::Kind, const Erased &) const final { return m_self(); }

protected:
  //  PRIVATE METHODS  //

  /// @brief Since this is our unset typing, values should not be subtyped from here.
  inline bool m_unify(const Erased &, Constraints *) const final { return false; }

  /**
   * @brief Handles printing the type.
   * @param os                        Output stream.
   */
  static inline void m_print(std::ostream &os, const Unset &) { os << "Unset"; }
};

} // namespace Sabre::Type

#endif
