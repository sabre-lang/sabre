#ifndef _SABRE_STATEMENT_BLOCK_HPP
#define _SABRE_STATEMENT_BLOCK_HPP

/// Sabre Includes
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Block Statement.
class Block : public Mixin<Block, Statement> {
  //  PROPERTIES  //

  /// @brief Available statements to be parsed.
  std::vector<Node *> m_statements;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a block statement.
   * @param statement                 Block node.
   * @param range                     Block range.
   */
  explicit Block(Node *statement) : Block(std::vector({statement})) {}

  /**
   * @brief Constructs a block statement.
   * @param statements                Block nodes.
   * @param range                     Block range.
   */
  template <std::derived_from<Node> T = Node>
  explicit Block(const std::vector<T *> &statements = {}) : m_statements($::Ranges::To<Node *>(statements)) {}

  //  PUBLIC METHODS  //

  /// @brief Denotes if the block is empty.
  inline constexpr bool empty() const noexcept { return m_statements.empty(); }

  /// @brief Gets the total size of the block.
  inline constexpr size_t size() const noexcept { return m_statements.size(); }

  /// @brief Gets the available statements.
  inline constexpr std::vector<Node *> &statements() noexcept { return m_statements; }
  inline constexpr const std::vector<Node *> &statements() const noexcept { return m_statements; }
};

} // namespace Sabre::Syntax

#endif
