#ifndef _SABRE_FORWARD_MACHINE_HPP
#define _SABRE_FORWARD_MACHINE_HPP

/// Vendor Includes
#include <xasm/xasm.hpp>

/// Sabre Includes
#include "sabre/forward/runtime.hpp"
#include "sabre/function/info.hpp"
#include "sabre/value/pointer.hpp"

/// Forward Declarations
$_FWD(Sabre::Machine, class Info)
$_FWD(Sabre::Machine, class Facts)
$_FWD(Sabre::Machine, class Frame)
$_FWD(Sabre::Machine, class Logger)
$_FWD(Sabre::Machine, class Service)
$_FWD(Sabre::Machine, class Emitter)
$_FWD(Sabre::Machine, struct Builder)
$_FWD(Sabre::Machine, class Allocator)

/// Forward Definitions
$_FWD(Sabre::Machine, using Stack = Value::Any *)
$_FWD(Sabre::Machine, using Label = XASM::Host::Label)
$_FWD(Sabre::Machine, using Double = XASM::Host::Double)
$_FWD(Sabre::Machine, using Memory = XASM::Host::Memory)
$_FWD(Sabre::Machine, using Immediate = XASM::Host::Immediate)
$_FWD(Sabre::Machine, using Reference = Pointer::Underlying)
$_FWD(Sabre::Machine, using Compiler = XASM::Compiler::Universal)

#endif
