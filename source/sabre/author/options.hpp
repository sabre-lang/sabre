#ifndef _SABRE_AUTHOR_OPTIONS_HPP
#define _SABRE_AUTHOR_OPTIONS_HPP

/// Sabre Includes
#include "sabre/forward/author.hpp"

namespace Sabre::Author {

/// @brief Docification Options.
struct Options {
  //  PROPERTIES  //

  /// @brief The current output file.
  $::String::Buffer output = {};
};

} // namespace Sabre::Author

#endif
