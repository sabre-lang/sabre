/// Builtin Includes
#include "sabre/builtins/_inline/builtins.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Definitions
$_FWD(Sabre::Builtins, using TN = Type::New)

//  TYPEDEFS  //

#define SABRE_XX_FIELDS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Field, static Type::Entity N())
#include "sabre/builtins/function/_defines/fields.def"

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Type::Entity N())
#include "sabre/builtins/function/_defines/statics.def"

//  PUBLIC METHODS  //

$::Shared::Pointer<Sabre::Type::Prototype> Sabre::Builtins::Wrapper<Sabre::Function::Any>::typeclass() {
  return m_typeclass([](const auto &) {});
}

Sabre::Type::Entity Sabre::Builtins::Field::arity() { return TN::function(TN::number()); }
Sabre::Type::Entity Sabre::Builtins::Field::adicity() { return TN::function(TN::number()); }
Sabre::Type::Entity Sabre::Builtins::Field::receiver() { return TN::function(TN::any()); }

Sabre::Type::Entity Sabre::Builtins::Static::vlimit() { return TN::function(TN::number()); }

Sabre::Type::Entity Sabre::Builtins::Static::bind() {
  auto F = TN::constraint("F", TN::variadic());
  return TN::generic(TN::function(F, F, TN::any()), F);
}

Sabre::Type::Entity Sabre::Builtins::Static::call() {
  auto F = TN::constraint("F", TN::variadic());
  return TN::generic(TN::invocation(F, false), F);
}

Sabre::Type::Entity Sabre::Builtins::Static::apply() {
  auto passthrough = TN::optional(TN::list(TN::any()));
  return TN::function(TN::any(), TN::variadic(), passthrough);
}

//  PRIVATE METHODS  //

void Sabre::Builtins::Wrapper<Sabre::Function::Any>::m_typedefs(Type::World *globals) {
  // prepare the baseline details
  auto prototype = typeclass();
  auto &fields = prototype->fields();
  auto &statics = prototype->statics();

// define the fields for symbols
#define SABRE_XX_FIELDS_DEFINE(N, ...) fields.emplace(#N, Field::N());
#include "sabre/builtins/function/_defines/fields.def"

// define the statics for symbols
#define SABRE_XX_STATICS_DEFINE(N, ...) statics.emplace(#N, Static::N());
#include "sabre/builtins/function/_defines/statics.def"

  // define the baseline types
  globals->values().declare(name(), prototype);
  globals->types().declare(name(), TN::variadic());
}
