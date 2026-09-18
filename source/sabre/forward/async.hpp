#ifndef _SABRE_FORWARD_ASYNC_HPP
#define _SABRE_FORWARD_ASYNC_HPP

/// Vendor Includes
#include <xinv/xinv.hpp>
#include <xsio/xsio.hpp>

/// Forward Declarations
$_FWD(Sabre::Async, struct Entry)
$_FWD(Sabre::Async, class Service)
$_FWD(Sabre::Async, struct Future)
$_FWD(Sabre::Async, struct Thenable)

/// Forward Definitions
$_FWD(Sabre::Async, using Options = XSIO::Async::Options)
$_FWD(Sabre::Async, using Thread = XSIO::Virtual::Thread)
$_FWD(Sabre::Async, using Scheduler = XSIO::Async::Scheduler)
$_FWD(Sabre::Async, using Processor = XSIO::Virtual::Processor)
$_FWD(Sabre::Async, using Timeout = $::Unique::Pointer<XSIO::Task::Timeout>)
$_FWD(Sabre::Async, template <class T> using Task = XSIO::Task::Mixin<T>)

#endif
