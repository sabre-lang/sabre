#ifndef _SABRE_SYNTAX_GENERATOR_IPP
#define _SABRE_SYNTAX_GENERATOR_IPP

/// Sabre Includes
#include "sabre/syntax/grammar.hpp"
#include "sabre/syntax/visitor.hpp"

namespace Sabre::Syntax {

/// @brief Inline Syntax Generator.
struct Generator {
  //  PUBLIC METHODS  //

  /// @brief Handles generating an expression generator.
  template <class T, class P, class D> static inline constexpr Syntax::Grammar::Table<T, P> grammar() {
    // prepare the baseline constructor to be used
    using K = Lexer::Kind;
    using R = Syntax::Grammar::Rule<T, P>;

    // construct the table to be output here
    static auto s_table = Syntax::Grammar::Table<T, P>();
    static auto _ = [](K kind) { return static_cast<size_t>(kind); };

    // prepare all the identifiers to be bound
    s_table[_(K::LTRL_VOID)] = R(D::template m_prefix<Syntax::Void>);
    s_table[_(K::LTRL_TRUE)] = R(D::template m_prefix<Syntax::True>);
    s_table[_(K::LTRL_FALSE)] = R(D::template m_prefix<Syntax::False>);
    s_table[_(K::LTRL_STR)] = R(D::template m_prefix<Syntax::Text>);
    s_table[_(K::LTRL_SELF)] = R(D::template m_prefix<Syntax::Self>);
    s_table[_(K::LTRL_IDENT)] = R(D::template m_prefix<Syntax::Identifier>);
    s_table[_(K::DECL_FUNC)] = R(D::template m_prefix<Syntax::Lambda>);

    // prepare all the numerics to be bound
    s_table[_(K::LTRL_BIN)] = R(D::template m_prefix<Syntax::Numeric>);
    s_table[_(K::LTRL_OCT)] = R(D::template m_prefix<Syntax::Numeric>);
    s_table[_(K::LTRL_HEX)] = R(D::template m_prefix<Syntax::Numeric>);
    s_table[_(K::LTRL_INT)] = R(D::template m_prefix<Syntax::Numeric>);
    s_table[_(K::LTRL_FLT)] = R(D::template m_prefix<Syntax::Numeric>);
    s_table[_(K::LTRL_NAN)] = R(D::template m_prefix<Syntax::Numeric>);
    s_table[_(K::LTRL_INF)] = R(D::template m_prefix<Syntax::Numeric>);

    // prepare all the specialized expressions to be bound
    s_table[_(K::FLOW_PANIC)] = R(D::template m_prefix<Syntax::Panic>);
    s_table[_(K::PUNC_POLICY)] = R(D::template m_infix<Syntax::Execute>, Precedence::CALL);
    s_table[_(K::PUNC_COMMA)] = R(D::template m_infix<Syntax::Binary>, Precedence::COMMA);
    s_table[_(K::PUNC_PERIOD)] = R(D::template m_infix<Syntax::Accessor>, Precedence::CALL);
    s_table[_(K::PUNC_QUERY)] = R(D::template m_infix<Syntax::Ternary>, Precedence::TERNARY);

    s_table[_(K::PUNC_LBRACE)] = R(D::template m_prefix<Syntax::Record>);
    s_table[_(K::PUNC_LPAREN)] =
        R(D::template m_prefix<Syntax::Group>, D::template m_infix<Syntax::Call>, Precedence::CALL);
    s_table[_(K::PUNC_LBRACK)] =
        R(D::template m_prefix<Syntax::Tuple>, D::template m_infix<Syntax::Typed>, Precedence::TMPL);

#define SABRE_XX_TOKEN_UNARY(N, ...) s_table[_(K::UNOP_##N)] = R(D::template m_prefix<Syntax::Unary>);
#include "sabre/lexer/_defines/tokens.def"

#define SABRE_XX_TOKEN_BINARY(N, S, P, ...)                                         \
  s_table[_(K::BINOP_##N)] = R(D::template m_infix<Syntax::Binary>, Precedence::P);
#include "sabre/lexer/_defines/tokens.def"

    // prepare the inference handlers to be used
    s_table[_(K::BINOP_AS)] = R(D::template m_infix<Syntax::Cast>, Precedence::INFER);
    s_table[_(K::BINOP_IS)] = R(D::template m_infix<Syntax::Infer>, Precedence::INFER);

    // override the "xor" handler with a secondary prefix handler
    s_table[_(K::BINOP_XOR)].prefix = D::template m_prefix<Syntax::Caret>;

    // override the "sub" handler with a secondary prefix handler
    s_table[_(K::BINOP_SUB)].prefix = D::template m_prefix<Syntax::Unary>;

    // and return the resulting table now
    return s_table;
  }

  /**
   * @brief Handles parsing expressions.
   * @param parser                  Parser stream.
   * @param rank                    Precendence rank.
   * @param bail                    Bailout callback.
   */
  template <class T, class P, class D, class B> static inline T *expression(P *parser, Precedence rank, const B &bail) {
    // prepare the grammar to be used for dispatching
    static auto s_grammar = grammar<T, P, D>();

    // attempt getting an initial prefix value to begin with
    auto kind = static_cast<size_t>(parser->current()->kind());
    auto rule = parser->eos() ? Grammar::Rule<T, P>() : s_grammar.at(kind);
    if (rule.prefix == nullptr) return bail(parser, 2000100, "an expression");

    // check if assignment is valid now
    bool assignable = rank <= Precedence::ASSIGN;

    // get the result of the prefix handle
    auto node = rule.prefix(parser, assignable);

    // attempt parsing infix expressions now
    for (; node;) {
      kind = static_cast<size_t>(parser->current()->kind()); // get next
      rule = parser->eos() ? Grammar::Rule<T, P>() : s_grammar.at(kind);

      // stop when the current rank exceeds the next rule
      if (rank > rule.rank) break;

      // ensure that we have a valid rule available for this infix expression
      $_ASSERT(rule.infix, "Unknown infix rule for '{0}'", Lexer::Inspect::name(static_cast<Lexer::Kind>(kind)));

      // call the infix handler now to be used
      node = rule.infix(parser, node, assignable);
    }

    // stop early when we have invalid nodes incoming
    if ($_UNLIKELY(node == nullptr)) return nullptr;

    // check for items that are validly assignable now
    auto invalid = assignable && parser->check(Lexer::Flag::ASSIGNS);
    return invalid ? bail(parser, node, 2000500) : node;
  }
};

} // namespace Sabre::Syntax

#endif
