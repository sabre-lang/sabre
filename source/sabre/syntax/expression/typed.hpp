#ifndef _SABRE_EXPRESSION_TYPED_HPP
#define _SABRE_EXPRESSION_TYPED_HPP

/// Sabre Syntax
#include "sabre/syntax/annotation/generics.hpp"

namespace Sabre::Syntax {

/// @brief Typed Expression Node.
class Typed : public Mixin<Typed, Expression> {
  //  PROPERTIES  //

  /// @brief Expression to type.
  Expression *m_value;

  /// @brief Available type-arguments.
  Specialization m_types;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a typed expression.
   * @param value                Expression value.
   * @param arguments                 Type arguments.
   */
  explicit Typed(Expression *value, const Specialization &types = {}) : m_value(value), m_types(types) {}

  //  PUBLIC METHODS  //

  inline constexpr Expression *value() const noexcept { return m_value; }
  inline constexpr const Specialization &types() const noexcept { return m_types; }
};

} // namespace Sabre::Syntax

#endif
