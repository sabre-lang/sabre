#ifndef _SABRE_SERVER_OPTIONS_HPP
#define _SABRE_SERVER_OPTIONS_HPP

/// Sabre Includes
#include "sabre/forward/server.hpp"
#include "sabre/product/semver.hpp"

namespace Sabre::Server {

/// @brief Underlying server options.
struct Options : public XLSP::Server::Options {
  //  CONSTRUCTORS  //

  /// @brief Ensures underlying options are set properly.
  constexpr Options() {
    server.title = SABRE_MM_TITLE;
    server.version = SABRE_MM_REVISION;
  }
};

} // namespace Sabre::Server

#endif
