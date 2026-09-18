#ifndef _SABRE_BUNDLE_FUSE_HPP
#define _SABRE_BUNDLE_FUSE_HPP

/// Sabre Includes
#include "sabre/forward/bundle.hpp"

/**
 * The fuses generated in this file should contain:
 * N: Name of the fuse (typically in uppercase).
 * F: A random hex-string (32-characters long).
 */

//  X-MACROS  //

#define SABRE_XX_FUSES_LIST(X) X(BUNDLED, "a8c0c5af64a6e91209ca2d3821c045ca")

//  NAMESPACES  //

namespace Sabre::Fuse {

#define X(N, ...) extern XJCT::Sentinel::View N;
SABRE_XX_FUSES_LIST(X)
#undef X

} // namespace Sabre::Fuse

#endif
