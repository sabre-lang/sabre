#ifndef _SABRE_EXPRESSION_PANIC_HPP
#define _SABRE_EXPRESSION_PANIC_HPP

/// Sabre Includes
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Panic Expression.
class Panic : public Mixin<Panic, Expression> {
  //  PROPERTIES  //

  /// @brief Associated statement value.
  Expression *m_value;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a "panic" expression.
   * @param value                     Failure value.
   */
  explicit Panic(Expression *value) : m_value(value) {}

  //  PUBLIC METHODS  //

  /// @brief Encpasulated panic value.
  inline constexpr const Expression *value() const noexcept { return m_value; }
};

} // namespace Sabre::Syntax

#endif
