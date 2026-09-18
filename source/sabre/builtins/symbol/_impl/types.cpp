/// Builtin Includes
#include "sabre/builtins/_inline/builtins.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Definitions
$_FWD(Sabre::Builtins, using TN = Type::New)

//  TYPEDEFS  //

#define SABRE_XX_FIELDS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Field, static Type::Entity N())
#include "sabre/builtins/symbol/_defines/fields.def"

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Type::Entity N())
#include "sabre/builtins/symbol/_defines/statics.def"

//  PUBLIC METHODS  //

$::Shared::Pointer<Sabre::Type::Prototype> Sabre::Builtins::Wrapper<Sabre::Value::Symbol>::typeclass() {
  return m_typeclass([](const auto &) {});
}

Sabre::Type::Entity Sabre::Builtins::Field::hash() { return TN::function(TN::number()); }
Sabre::Type::Entity Sabre::Builtins::Static::from() { return TN::function(TN::symbol(), TN::any()); }

//  PRIVATE METHODS  //

void Sabre::Builtins::Wrapper<Sabre::Value::Symbol>::m_typedefs(Type::World *globals) {
  // prepare the baseline details
  auto prototype = typeclass();
  auto &fields = prototype->fields();
  auto &statics = prototype->statics();

// define the fields for symbols
#define SABRE_XX_FIELDS_DEFINE(N, ...) fields.emplace(#N, Field::N());
#include "sabre/builtins/symbol/_defines/fields.def"

// define the statics for symbols
#define SABRE_XX_STATICS_DEFINE(N, ...) statics.emplace(#N, Static::N());
#include "sabre/builtins/symbol/_defines/statics.def"

  // define the baseline types
  globals->values().declare(name(), prototype);
  globals->types().declare(name(), TN::symbol());
}
