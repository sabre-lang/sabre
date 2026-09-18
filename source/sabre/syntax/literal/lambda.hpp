#ifndef _SABRE_LITERAL_LAMBDA_HPP
#define _SABRE_LITERAL_LAMBDA_HPP

/// Syntax Includes
#include "sabre/syntax/annotation/signature.hpp"

namespace Sabre::Syntax {

/// @brief Lambda Literal Node.
class Lambda : public Mixin<Lambda, Expression> {
  //  PROPERTIES  //

  /// @brief The function body.
  Node *m_body;

  /// @brief Associated function signature.
  Signature *m_signature;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a lambda expression.
   * @param signature                 Lambda signature.
   * @param body                      Function body.
   */
  explicit Lambda(Signature *signature, Node *body) : m_body(body), m_signature(signature) {}

  //  PUBLIC METHODS  //

  inline constexpr const Node *body() const noexcept { return m_body; }
  inline constexpr const Signature *signature() const noexcept { return m_signature; }
  inline constexpr const Parameters::List &parameters() const noexcept { return m_signature->parameters(); }

  inline constexpr size_t arity() const noexcept { return m_signature->arity(); }
  inline constexpr size_t adicity() const noexcept { return m_signature->adicity(); }
};

} // namespace Sabre::Syntax

#endif
