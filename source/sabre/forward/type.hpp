#ifndef _SABRE_FORWARD_TYPE_HPP
#define _SABRE_FORWARD_TYPE_HPP

/// Vendor Includes
#include <xinv/xinv.hpp>

/// Sabre Includes
#include "sabre/forward/flow.hpp"
#include "sabre/operator/kind.hpp"

/// Forward Declarations
$_FWD(Sabre::Type, class Exports)
$_FWD(Sabre::Type, class Analyzer)
$_FWD(Sabre::Type, struct Match)
$_FWD(Sabre::Type, struct Deduction)
$_FWD(Sabre::Type, struct Narrowing)

$_FWD(Sabre::Type, struct New)
$_FWD(Sabre::Type, struct Unit)
$_FWD(Sabre::Type, class Entity)
$_FWD(Sabre::Type, class World)

$_FWD(Sabre::Type, struct Any)
$_FWD(Sabre::Type, struct None)
$_FWD(Sabre::Type, struct Unset)
$_FWD(Sabre::Type, struct Poison)

$_FWD(Sabre::Type, class Callable)
$_FWD(Sabre::Type, class Instance)
$_FWD(Sabre::Type, class Structure)
$_FWD(Sabre::Type, class Prototype)
$_FWD(Sabre::Type, class Generic)
$_FWD(Sabre::Type, class Parameter)
$_FWD(Sabre::Type, class Transform)

/// Forward Definitions
$_FWD(Sabre::Type, using Erased = $::Shared::Pointer<Unit>)
$_FWD(Sabre::Type, using Control = $::Unique::Pointer<Flow::Control>)
$_FWD(Sabre::Type, using Deferrer = $::Shared::Functor<void(Analyzer *)>)
$_FWD(Sabre::Type, using Branch = $::Unique::Functor<Control(Analyzer *)>)
$_FWD(Sabre::Type, using Constraints = $::Map::Base<const Parameter *, Erased>)
$_FWD(Sabre::Type, using Template = std::vector<$::Shared::Pointer<Parameter>>)
$_FWD(Sabre::Type, using Resolver = $::Shared::Functor<Erased(const Erased &, Constraints *) const>)
$_FWD(Sabre::Type, using Apply = $::Shared::Functor<Erased(const Structure *, Operator::Kind, const Erased &) const>)

#endif
