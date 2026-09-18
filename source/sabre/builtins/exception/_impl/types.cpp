/// Builtin Includes
#include "sabre/builtins/_inline/builtins.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Definitions
$_FWD(Sabre::Builtins, using TN = Type::New)

//  TYPEDEFS  //

#define SABRE_XX_FIELDS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Field, static Type::Entity N())
#include "sabre/builtins/exception/_defines/fields.def"

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Type::Entity N())
#include "sabre/builtins/exception/_defines/statics.def"

//  PUBLIC METHODS  //

$::Shared::Pointer<Sabre::Type::Prototype> Sabre::Builtins::Wrapper<Sabre::Object::Exception>::typeclass() {
  return m_typeclass([](const auto &) {});
}

Sabre::Type::Entity Sabre::Builtins::Field::name() { return TN::function(TN::string()); }
Sabre::Type::Entity Sabre::Builtins::Field::message() { return TN::function(TN::string()); }

Sabre::Type::Entity Sabre::Builtins::Static::from() {
  return TN::variadic(TN::exception(), TN::optional(TN::string()), TN::any());
}

Sabre::Type::Entity Sabre::Builtins::Static::named() {
  auto format = TN::optional(TN::string()); // optional format argument
  auto args = std::vector<Type::Entity>({TN::string(), format, TN::any()});
  return TN::variadic(TN::exception(), args); // construct the result
}

//  PRIVATE METHODS  //

void Sabre::Builtins::Wrapper<Sabre::Object::Exception>::m_typedefs(Type::World *globals) {
  // prepare the baseline details
  auto prototype = typeclass();
  auto &fields = prototype->fields();
  auto &statics = prototype->statics();

// define the fields for symbols
#define SABRE_XX_FIELDS_DEFINE(N, ...) fields.emplace(#N, Field::N());
#include "sabre/builtins/exception/_defines/fields.def"

// define the statics for symbols
#define SABRE_XX_STATICS_DEFINE(N, ...) statics.emplace(#N, Static::N());
#include "sabre/builtins/exception/_defines/statics.def"

  // define the baseline types
  globals->values().declare(name(), prototype);
  globals->types().declare(name(), TN::exception());
}
