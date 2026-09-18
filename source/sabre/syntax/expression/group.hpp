#ifndef _SABRE_EXPRESSION_GROUP_HPP
#define _SABRE_EXPRESSION_GROUP_HPP

/// Sabre Includes
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Group Expression.
class Group : public Mixin<Group, Expression> {
  //  PROPERTIES  //

  /// @brief Associated statement value.
  Expression *m_value;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a grouped expression.
   * @param value                     Grouped value.
   */
  explicit Group(Expression *value) : m_value(value) {}

  //  PUBLIC METHODS  //

  /// @brief Encpasulated panic value.
  inline constexpr const Expression *value() const noexcept { return m_value; }
};

} // namespace Sabre::Syntax

#endif
