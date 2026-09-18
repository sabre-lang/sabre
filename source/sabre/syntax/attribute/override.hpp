#ifndef _SABRE_ATTRIBUTE_OVERRIDE_HPP
#define _SABRE_ATTRIBUTE_OVERRIDE_HPP

/// Sabre Includes
#include "sabre/operator/inspect.hpp"

/// Syntax Includes
#include "sabre/syntax/declaration/attribute.hpp"

namespace Sabre::Syntax {

/// @brief Compile Time Operator Node.
class Override : public Mixin<Override, Attribute> {
  //  PROPERTIES  //

  /// @brief The underlying operator kind.
  Operator::Kind m_kind;

  /// @brief Allow setting the target.
  Expression *m_target;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an operator overload.
   * @param kind              Kind of operator.
   * @param target            Target to overload.
   */
  explicit Override(Operator::Kind kind, Expression *target = nullptr) :
      Mixin(Reflect::Category::OPERATOR), m_kind(kind), m_target(target) {}

  //  PUBLIC METHODS  //

  inline constexpr Operator::Kind kind() const noexcept { return m_kind; }
  inline constexpr const Expression *target() const noexcept { return m_target; }
  inline constexpr $::String::View label() const noexcept { return ::Sabre::Operator::Inspect::name(m_kind); }
  inline constexpr $::String::View symbol() const noexcept { return ::Sabre::Operator::Inspect::symbol(m_kind); }
};

} // namespace Sabre::Syntax

#endif
