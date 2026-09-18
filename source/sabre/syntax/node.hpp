#ifndef _SABRE_SYNTAX_NODE_HPP
#define _SABRE_SYNTAX_NODE_HPP

/// Sabre Includes
#include "sabre/syntax/trivia.hpp"

namespace Sabre::Syntax {

/// @brief Node Extension Mixin.
template <class T, class B = Node> using Mixin = $::RTTI::Mixin<T, B>;

/// @brief Node Abstraction.
class $_ABSTRACT Node : public $::RTTI::Dynamic {
  //  TYPEDEFS  //

  /// @brief Allow the trivia handler internal access.
  friend class Trivia;

  /// @brief Allow the syntax storage internal access.
  friend class Storage;

  //  PROPERTIES  //

  /// @brief Internal syntax metadata proxy.
  $::Unique::Pointer<Trivia> m_trivia = $::Unique::New<Trivia>();

public:
  //  CONSTRUCTORS  //

  /// @brief Default constructor for nodes.
  constexpr Node() = default;

  /**
   * @brief Helper for constructing trivia nodes.
   * @param keyid                 Key identifier to bind.
   */
  template <std::derived_from<Node> T> constexpr Node(const T *keyid) : m_trivia($::Unique::New<Trivia>(keyid)) {}

  /// @brief Virtual abstract destructor.
  virtual ~Node() = default;

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying node trivia.
  inline constexpr Trivia *trivia() const noexcept { return m_trivia.get(); }

  /// @brief Downcasts nodes to derived types.
  template <std::derived_from<Node> T> inline constexpr T *as() noexcept { return $::RTTI::Cast<T>(this); }
  template <std::derived_from<Node> T> inline constexpr const T *as() const noexcept { return $::RTTI::Cast<T>(this); }
};

/// @brief Baseline Expression Node.
struct Expression : public Mixin<Expression> {
  using Mixin<Expression, Node>::Mixin;
};

/// @brief Baseline Statement Node.
struct Statement : public Mixin<Statement> {
  using Mixin<Statement, Node>::Mixin;
};

/// @brief Baseline Annotation Node.
struct Annotation : public Mixin<Annotation> {
  using Mixin<Annotation, Node>::Mixin;
};

/// @brief Fallback Expression Node.
struct Fallback : public Mixin<Fallback, Expression> {
  explicit Fallback() = default;
};

} // namespace Sabre::Syntax

#endif
