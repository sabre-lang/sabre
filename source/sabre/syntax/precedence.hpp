#ifndef _SABRE_SYNTAX_PRECEDENCE_HPP
#define _SABRE_SYNTAX_PRECEDENCE_HPP

/// Sabre Includes
#include "sabre/forward/syntax.hpp"

namespace Sabre::Syntax {

/// @brief Operator/Types Precedence Ranks.
enum class Precedence : uint8_t {
  NONE = 0,

  COMMA, // ,

  ASSIGN,           // =
  TERNARY = ASSIGN, // a ? b : c

  COAL,       // ??
  LOR = COAL, // ||

  LAND, // &&
  BOR,  // |
  XOR,  // ^
  BAND, // &

  EQUAL,   // ==, !=
  COMPARE, // < > <= >=

  SHIFT,    // << >>
  TERM,     // + -
  FACTOR,   // * / %
  EXPONENT, // **

  UNARY, // ! - ++ -- ~

  INFER, // as is
  CALL,  // . ?. :: ()
  TMPL,  // []
};

} // namespace Sabre::Syntax

#endif
