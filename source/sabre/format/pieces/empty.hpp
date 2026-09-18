#ifndef _SABRE_FORMAT_EMPTY_HPP
#define _SABRE_FORMAT_EMPTY_HPP

/// Format Includes
#include "sabre/format/pieces/unicode.hpp"

namespace Sabre::Format {

/// @brief Explicit Empty Node.
struct Piece::Empty : public Piece::Mixin<Empty, Unicode> {
  //  CONSTRUCTORS  //

  /// @brief A node that is always empty.
  explicit Empty() = default;
};

} // namespace Sabre::Format

#endif