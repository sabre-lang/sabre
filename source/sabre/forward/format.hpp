#ifndef _SABRE_FORWARD_FORMAT_HPP
#define _SABRE_FORWARD_FORMAT_HPP

/// Vendor Includes
#include <xtdlib/xtdlib.hpp>

/// Forward Declarations
$_FWD(Sabre::Format, class Reader)
$_FWD(Sabre::Format, class Writer)
$_FWD(Sabre::Format, struct Piece)
$_FWD(Sabre::Format, class Storage)
$_FWD(Sabre::Format, struct Options)
$_FWD(Sabre::Format, class Pipeline)

/// Forward Definitions
$_FWD(Sabre::Format, using Sigil = size_t)
$_FWD(Sabre::Format, using Buffer = std::vector<Piece *>)
$_FWD(Sabre::Format, using Result = std::optional<$::String::Buffer>)

#endif
