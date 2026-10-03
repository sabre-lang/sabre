/// Builtin Includes
#include "sabre/builtins/_inline/apply.ipp"
#include "sabre/builtins/_inline/builtins.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Definitions
$_FWD(Sabre::Builtins, using TN = Type::New)
$_FWD(Sabre::Builtins, using Self = Type::Structure)

//  TYPEDEFS  //

#define SABRE_XX_FIELDS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Field, static Type::Entity N())
#include "sabre/builtins/buffer/_defines/fields.def"

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Type::Entity N())
#include "sabre/builtins/buffer/_defines/statics.def"

//  PUBLIC METHODS  //

$::Shared::Pointer<Sabre::Type::Prototype> Sabre::Builtins::Wrapper<Sabre::Iterable::Buffer>::typeclass() {
  return m_typeclass([](const auto &) {});
}

Sabre::Type::Entity Sabre::Builtins::Field::size() { return TN::function(TN::number()); }
Sabre::Type::Entity Sabre::Builtins::Field::empty() { return TN::function(TN::boolean()); }

Sabre::Type::Entity Sabre::Builtins::Field::front() { return TN::function(TN::number()); }
Sabre::Type::Entity Sabre::Builtins::Field::back() { return TN::function(TN::number()); }

Sabre::Type::Entity Sabre::Builtins::Field::map() {
  // prepare the incoming values now
  auto index = TN::optional(TN::number());
  auto value = TN::optional(TN::number());

  // prepare the mapping signature
  auto callback = TN::function(TN::number(), value, index);
  return TN::function(TN::buffer(), callback);
}

Sabre::Type::Entity Sabre::Builtins::Field::fold() {
  auto V = TN::constraint("V", TN::any(), TN::any());
  auto callback = TN::function(V, V, TN::number());
  return TN::generic(TN::function(V, V, callback), V);
}

Sabre::Type::Entity Sabre::Builtins::Field::slice() {
  auto start = TN::optional(TN::number()), end = TN::optional(TN::number());
  return TN::function(TN::buffer(), start, end);
}

Sabre::Type::Entity Sabre::Builtins::Field::reverse() { return {TN::function(TN::buffer())}; }

Sabre::Type::Entity Sabre::Builtins::Field::first_index_of() { return TN::function(TN::number(), TN::number()); }
Sabre::Type::Entity Sabre::Builtins::Field::last_index_of() { return TN::function(TN::number(), TN::number()); }

Sabre::Type::Entity Sabre::Builtins::Static::empty() { return TN::function(TN::buffer()); }
Sabre::Type::Entity Sabre::Builtins::Static::filled() { return TN::function(TN::buffer(), TN::number(), TN::number()); }

template <>
Sabre::Type::Erased
Sabre::Builtins::Apply<Sabre::Iterable::Buffer>::unary(const Type::Structure *, Operator::Kind kind) {
  switch (kind) {
  case Operator::Kind::ITER: return TN::number();
  default: return TN::unset(); // resolve accordingly
  }
}

template <>
Sabre::Type::Erased
Sabre::Builtins::Apply<Sabre::Iterable::Buffer>::binary(const Type::Structure *, Operator::Kind, const Type::Erased &) {
  return TN::unset();
}

//  PRIVATE METHODS  //

void Sabre::Builtins::Wrapper<Sabre::Iterable::Buffer>::m_typedefs(Type::World *globals) {
  // prepare the baseline details
  auto prototype = typeclass();
  auto &fields = prototype->fields();
  auto &statics = prototype->statics();

  // bind the decision tree for operators
  prototype->operators() = Apply<Iterable::Buffer>::decide;

// define the fields for symbols
#define SABRE_XX_FIELDS_DEFINE(N, ...) fields.emplace(#N, Field::N());
#include "sabre/builtins/buffer/_defines/fields.def"

// define the statics for symbols
#define SABRE_XX_STATICS_DEFINE(N, ...) statics.emplace(#N, Static::N());
#include "sabre/builtins/buffer/_defines/statics.def"

  // define the baseline types
  globals->values().declare(name(), prototype);
  globals->types().declare(name(), prototype->instantiate());
}
