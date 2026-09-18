#ifndef _SABRE_SYNTAX_TREE_HPP
#define _SABRE_SYNTAX_TREE_HPP

/// Sabre Includes
#include "sabre/syntax/node.hpp"
#include "sabre/variable/captures.hpp"

/// Syntax Includes
#include "sabre/syntax/literal/lambda.hpp"
#include "sabre/syntax/statement/block.hpp"
#include "sabre/syntax/statement/import.hpp"

namespace Sabre::Syntax {

/// @brief Aliased Variable Captures.
using Captures = ::Sabre::Variable::Captures;

/// @brief Syntax Node Storage.
class Storage {
  //  TYPEDEFS  //

  /// @brief Allow syntax-trees internal access.
  friend class Tree;

  //  PROPERTIES  //

  /// @brief The storage reference for nodes.
  std::vector<$::Unique::Pointer<Node>> m_nodes = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a storage container.
  constexpr Storage() = default;

  /**
   * @brief Constructs a storage container.
   * @param capacity                Initial capacity.
   */
  constexpr Storage(size_t capacity) : Storage() { m_nodes.reserve(capacity); }

  //  PUBLIC METHODS  //

  /**
   * @brief Handles allocating a syntax-node.
   * @param bounds                  Bounds of node.
   * @param args                    Arguments to bind.
   */
  template <std::derived_from<Node> T, class B, class... As> inline constexpr T *allocate(B bounds, As &&...args) {
    // we start by constructing the node to be used now
    auto node = $::Unique::New<T>(std::forward<As>(args)...);

    // we then forcibly construct the nodes associated trivia and comments
    node->m_trivia = $::Unique::New<Trivia>(node.get(), bounds);

    // and lastly convert our node back to the required storage
    return static_cast<T *>(m_nodes.emplace_back(std::move(node)).get());
  }
};

/// @brief Syntax Tree Container.
class Tree : public Mixin<Tree> {
  //  PROPERTIES  //

  /// @brief The underlying main function node.
  Lambda *m_main = nullptr;

  /// @brief The syntax-tree block statements.
  Block *m_block = nullptr;

  /// @brief The storage reference for nodes.
  Storage m_storage = {};

  /// @brief The associated tree resource.
  $::URI::View m_resource = {};

  /// @brief Currently captured variable extents.
  Captures m_captures = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a syntax tree.
   * @param capacity                Initial capacity.
   * @param resource                Resource identifier.
   */
  explicit Tree(const $::URI::View &resource = {});
  explicit Tree(size_t capacity, const $::URI::View &resource = {});
  explicit Tree(const std::vector<Lexer::Token> &tokens, const $::URI::View &resource = {});

  //  PUBLIC METHODS  //

  /// @brief Gets the module entry-point function.
  inline constexpr Lambda *main() const noexcept { return m_main; }

  /// @brief Gets the resource view for the syntax-tree.
  inline constexpr $::URI::View resource() const noexcept { return m_resource; }

  /// @brief Gets the available variable captures.
  inline constexpr Captures *captures() noexcept { return &m_captures; }
  inline constexpr const Captures *captures() const noexcept { return &m_captures; }

  /// @brief Gets the syntax-trees available statements.
  inline constexpr std::vector<Node *> &statements() noexcept { return m_block->statements(); }
  inline constexpr const std::vector<Node *> &statements() const noexcept { return m_block->statements(); }

  /// @brief Storage container for nodes and their metadata.
  inline constexpr Storage &storage() noexcept { return m_storage; }
  inline constexpr const Storage &storage() const noexcept { return m_storage; }

  /// @brief Gets a list of all viable syntax dependencies (unresolved).
  inline constexpr Dependencies dependencies() const noexcept {
    // prepare the output value to be used
    auto output = Dependencies();

    // iterate over the available storage
    for (const auto &node : m_storage.m_nodes) {
      if (!node->is<Import>()) continue; // ignorable
      output.emplace_back(node->as<Import>()->path());
    }

    // return the final dependencies found
    return output;
  }

  /**
   * @brief Handles allocating a syntax-node.
   * @param args                  Arguments to bind.
   */
  template <std::derived_from<Node> T, class... As> inline constexpr T *allocate(As &&...args) {
    return m_storage.allocate<T>(Bounds(), std::forward<As>(args)...);
  }

  /**
   * @brief Handles allocating a syntax-node.
   * @param bounds                Bounds of node.
   * @param args                  Arguments to bind.
   */
  template <std::derived_from<Node> T, class... As> inline constexpr T *allocate(Bounds &&bounds, As &&...args) {
    return m_storage.allocate<T>(std::move(bounds), std::forward<As>(args)...);
  }

  /**
   * @brief Handles allocating a syntax-node.
   * @param bounds                Bounds of node.
   * @param args                  Arguments to bind.
   */
  template <std::derived_from<Node> T, class... As> inline constexpr T *allocate(const Bounds &bounds, As &&...args) {
    return m_storage.allocate<T>(bounds, std::forward<As>(args)...);
  }
};

} // namespace Sabre::Syntax

#endif
