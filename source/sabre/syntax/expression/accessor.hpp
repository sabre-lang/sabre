#ifndef _SABRE_EXPRESSION_ACCESSOR_HPP
#define _SABRE_EXPRESSION_ACCESSOR_HPP

/// Syntax Includes
#include "sabre/syntax/literal/identifier.hpp"

namespace Sabre::Syntax {

/// @brief Generic Accessor Expression.
class Accessor : public Mixin<Accessor, Expression> {
  //  PROPERTIES  //

  Identifier *m_field;  // Field identifier.
  Expression *m_parent; // Parent expression.

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a property accessor.
   * @param field                     Field expression.
   * @param parent                    Parent expression.
   */
  explicit Accessor(Identifier *field, Expression *parent) : m_field(field), m_parent(parent) {}

  //  PUBLIC METHODS  //

  inline constexpr const Identifier *field() const noexcept { return m_field; }
  inline constexpr const Expression *parent() const noexcept { return m_parent; }
};

} // namespace Sabre::Syntax

#endif
