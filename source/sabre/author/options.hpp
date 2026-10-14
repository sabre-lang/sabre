#ifndef _SABRE_AUTHOR_OPTIONS_HPP
#define _SABRE_AUTHOR_OPTIONS_HPP

/// Sabre Includes
#include "sabre/forward/author.hpp"

namespace Sabre::Author {

/// @brief Available Formats to Use.
enum class Format : uint8_t { JSON };

/// @brief Docification Options.
struct Options {
  //  PROPERTIES  //

  /// @brief Denotes using console instead.
  bool console = false;

  /// @brief Expected output format.
  Format format = Format::JSON;

  /// @brief The current output file.
  $::String::Buffer output = {};

  /// @brief Title for documentation output.
  std::optional<$::String::Buffer> title = std::nullopt;
};

} // namespace Sabre::Author

#endif
