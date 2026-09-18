#ifndef _SABRE_DECLARATION_NAMESPACE_HPP
#define _SABRE_DECLARATION_NAMESPACE_HPP

/// Syntax Includes
#include "sabre/syntax/declaration/preamble.hpp"
#include "sabre/syntax/statement/block.hpp"

namespace Sabre::Syntax {

/// @brief Namespace Declaration Node.
class Namespace : public Mixin<Namespace, Declaration> {
  //  PROPERTIES  //

  /// @brief Declaration block.
  Block *m_block;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a type declaration.
   * @param target                    Target name.
   * @param block                     Block statement.
   */
  explicit Namespace(const Lexer::Token *target, Block *block) : Mixin(target), m_block(block) {}

  /**
   * @brief Constructs a type declaration.
   * @param name                      Target name.
   * @param block                     Block statement.
   */
  explicit Namespace(const $::String::View &name, Block *block) : Mixin(name), m_block(block) {}

  //  PUBLIC METHODS  //

  /// @brief Namespace block statement.
  inline constexpr const Block *block() const noexcept { return m_block; }
};

} // namespace Sabre::Syntax

#endif
