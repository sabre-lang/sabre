#ifndef _SABRE_EXPRESSION_EXECUTE_HPP
#define _SABRE_EXPRESSION_EXECUTE_HPP

/// Syntax Includes
#include "sabre/syntax/expression/call.hpp"

namespace Sabre::Syntax {

/// @brief Execute Expression Node.
class Execute : public Mixin<Execute, Expression> {
  //  PROPERTIES  //

  /// @brief The call expression.
  Call *m_invocation;

  /// @brief Prepare the suitably function policy.
  Function::Policy m_policy;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a policy-based invocation.
   * @param invocation                Call node.
   * @param policy                    Function policy.
   */
  explicit Execute(Call *invocation, Function::Policy policy) : m_invocation(invocation), m_policy(policy) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the expected function policy.
  inline constexpr Function::Policy policy() const noexcept { return m_policy; }

  /// @brief The underlying call node.
  inline constexpr const Call *invocation() const noexcept { return m_invocation; }

  /// @brief Gets the calling expression.
  inline constexpr const Expression *callee() const noexcept { return m_invocation->callee(); }

  /// @brief Gets the bound arguments.
  inline constexpr const std::vector<Expression *> &arguments() const noexcept { return m_invocation->arguments(); }
};

} // namespace Sabre::Syntax

#endif
