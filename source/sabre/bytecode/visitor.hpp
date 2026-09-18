#ifndef _SABRE_BYTECODE_VISITOR_HPP
#define _SABRE_BYTECODE_VISITOR_HPP

/// Sabre Modules
#include "sabre/bytecode/allocator.hpp"
#include "sabre/bytecode/compiler.hpp"
#include "sabre/syntax/visitor.hpp"

/// Forward Declarations
$_FWD(Sabre::Bytecode, using Visitor = Syntax::Visitor<void, Compiler *, Register::Slot>)

//  MACROS  //

#define SABRE_MM_LOWER_NODE(T, N, C, D, ...)                         \
  template <>                                                        \
  void Sabre::Bytecode::Visitor::Acceptor::accept<Sabre::Syntax::T>( \
      const Syntax::T *N, Bytecode::Compiler *C, Register::Slot D    \
  )

//  NAMESPACES  //

namespace Sabre::Syntax {

/// @brief Visitor Specialization.
template <> struct Visitor<void, Bytecode::Compiler *, Register::Slot>::Acceptor {
  //  TYPEDEFS  //

  /// @brief Allow fast access to syllables.
  using Glyph = Bytecode::Glyph;

  //  PUBLIC METHODS  //

  template <std::derived_from<Node> T>
  static void accept(const T *node, Bytecode::Compiler *compiler, Register::Slot destination);
};

} // namespace Sabre::Syntax

//  SPECIALIZATIONS  //

#define X(T, ...) SABRE_MM_LOWER_NODE(T, , , );
SABRE_XX_SYNTAX_NODES(X)
#undef X

#endif
