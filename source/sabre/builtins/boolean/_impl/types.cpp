/// Builtin Includes
#include "sabre/builtins/_inline/builtins.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Definitions
$_FWD(Sabre::Builtins, using TN = Type::New)

//  TYPEDEFS  //

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Type::Entity N())
#include "sabre/builtins/boolean/_defines/statics.def"

//  PUBLIC METHODS  //

Sabre::Type::Entity Sabre::Builtins::Static::from() { return TN::function(TN::boolean(), TN::any()); }
Sabre::Type::Entity Sabre::Builtins::Static::parse() {
  auto sensitivity = TN::optional(TN::boolean()); // prepare
  return TN::function(TN::boolean(), TN::string(), sensitivity);
}

$::Shared::Pointer<Sabre::Type::Prototype> Sabre::Builtins::Wrapper<Sabre::Value::Boolean>::typeclass() {
  return m_typeclass([](const auto &) {});
}

//  PRIVATE METHODS  //

void Sabre::Builtins::Wrapper<Sabre::Value::Boolean>::m_typedefs(Type::World *globals) {
  // get the underlying prototype instance
  auto prototype = typeclass();
  auto &statics = prototype->statics();

  // define the underlying statics for booleans
#define SABRE_XX_STATICS_DEFINE(N, ...) statics.emplace(#N, Static::N());
#include "sabre/builtins/boolean/_defines/statics.def"

  // prepare the baseline typing to be used
  globals->values().declare(name(), prototype);
  globals->types().declare(name(), TN::boolean());

  // and generate the "True" and "False" typings as well
  globals->values().declare("True", TN::boolean());
  globals->values().declare("False", TN::boolean());
}
