#ifndef _SABRE_FORWARD_SERVER_HPP
#define _SABRE_FORWARD_SERVER_HPP

/// Vendor Includes
#include <xinv/xinv.hpp>
#include <xlsp/xlsp.hpp>
#include <xsio/xsio.hpp>

/// Forward Declarations
$_FWD(Sabre::Server, class Events)
$_FWD(Sabre::Server, class Worker)
$_FWD(Sabre::Server, struct Options)
$_FWD(Sabre::Server, class Utilities)
$_FWD(Sabre::Server, class Transport)
$_FWD(Sabre::Server, class Connection)

/// Forward Definitions
$_FWD(Sabre::Server, template <size_t N> using Channel = XLSP::Message::Channel<N>)

#endif
