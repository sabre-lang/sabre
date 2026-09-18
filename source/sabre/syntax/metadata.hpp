#ifndef _SABRE_SYNTAX_METADATA_HPP
#define _SABRE_SYNTAX_METADATA_HPP

/// Sabre Includes
#include "sabre/lexer/buffer.hpp"
#include "sabre/module/metadata.hpp"
#include "sabre/syntax/tree.hpp"

namespace Sabre::Module {

/// @brief Constructs a set of syntax metadata.
template <> class Metadata::Wrapper<Phase::PARSED> : public Metadata {
  //  PROPERTIES  //

  /// @brief Associated tokens buffer.
  Lexer::Buffer m_tokens = Lexer::Buffer();

  /// @brief Associated syntax tree.
  $::Unique::Pointer<Syntax::Tree> m_tree = $::Unique::New<Syntax::Tree>();

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a set of exports.
  explicit Wrapper() = default;

  //  PUBLIC METHODS  //

  inline constexpr Lexer::Buffer &tokens() noexcept { return m_tokens; }
  inline constexpr const Lexer::Buffer &tokens() const noexcept { return m_tokens; }

  inline constexpr $::Unique::Pointer<Syntax::Tree> &tree() noexcept { return m_tree; }
  inline constexpr const Syntax::Tree *tree() const noexcept { return m_tree.get(); }
};

} // namespace Sabre::Module

#endif
