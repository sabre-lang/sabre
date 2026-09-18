/// Builtin Includes
#include "sabre/builtins/_inline/builtins.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Definitions
$_FWD(Sabre::Builtins, using TN = Type::New)

//  TYPEDEFS  //

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Type::Entity N())
#include "sabre/builtins/enum/_defines/statics.def"

//  PUBLIC METHODS  //

$::Shared::Pointer<Sabre::Type::Prototype> Sabre::Builtins::Wrapper<Sabre::Object::Enum>::typeclass() {
  return m_typeclass([](const $::Shared::Pointer<Type::Prototype> &prototype) {
    // update the underlying super-type to be used
    prototype->super() = Wrapper<Number::Tagged>::typeclass();
  });
}

Sabre::Type::Entity Sabre::Builtins::Static::name() { return label(); }
Sabre::Type::Entity Sabre::Builtins::Static::label() {
  auto E = TN::constraint("E"); // prepare the constraint
  auto signature = TN::function(TN::string(), E, TN::number());
  return TN::generic(signature, E); // build the generic now
}

Sabre::Type::Entity Sabre::Builtins::Static::value() {
  auto E = TN::constraint("E"); // prepare the constraint
  auto signature = TN::function(TN::number(), E, TN::number());
  return TN::generic(signature, E); // build the generic now
}

//  PRIVATE METHODS  //

void Sabre::Builtins::Wrapper<Sabre::Object::Enum>::m_typedefs(Type::World *globals) {
  // get the underlying prototype instance
  auto prototype = typeclass();
  auto instance = prototype->instantiate();
  auto &statics = prototype->statics();

// define the underlying statics for enumerations
#define SABRE_XX_STATICS_DEFINE(N, ...) statics.emplace(#N, Static::N());
#include "sabre/builtins/enum/_defines/statics.def"

  // and define the baseline typings now
  globals->values().declare(name(), prototype);
  globals->types().declare(name(), instance);
}
