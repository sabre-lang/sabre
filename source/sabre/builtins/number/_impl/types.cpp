/// Sabre Includes
#include "sabre/number/tagged.hpp"

/// Builtin Includes
#include "sabre/builtins/_inline/apply.ipp"
#include "sabre/builtins/_inline/builtins.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Definitions
$_FWD(Sabre::Builtins, using TN = Type::New)

//  TYPEDEFS  //

#define SABRE_XX_FIELDS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Field, static Type::Entity N())
#include "sabre/builtins/number/_defines/fields.def"

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Type::Entity N())
#include "sabre/builtins/number/_defines/statics.def"

//  PUBLIC METHODS  //

$::Shared::Pointer<Sabre::Type::Prototype> Sabre::Builtins::Wrapper<Sabre::Number::Tagged>::typeclass() {
  return m_typeclass([](const auto &) {});
}

Sabre::Type::Entity Sabre::Builtins::Field::sign() { return TN::function(TN::number()); }
Sabre::Type::Entity Sabre::Builtins::Field::abs() { return TN::function(TN::number()); }
Sabre::Type::Entity Sabre::Builtins::Field::ceil() { return TN::function(TN::number()); }
Sabre::Type::Entity Sabre::Builtins::Field::floor() { return TN::function(TN::number()); }
Sabre::Type::Entity Sabre::Builtins::Field::round() { return TN::function(TN::number()); }
Sabre::Type::Entity Sabre::Builtins::Field::trunc() { return TN::function(TN::number()); }
Sabre::Type::Entity Sabre::Builtins::Field::clamp() {
  auto minimum = TN::optional(TN::number());
  auto maximum = TN::optional(TN::number());
  return TN::function(TN::number(), minimum, maximum);
}

Sabre::Type::Entity Sabre::Builtins::Field::is_nan() { return TN::function(TN::boolean()); }
Sabre::Type::Entity Sabre::Builtins::Field::is_fin() { return TN::function(TN::boolean()); }
Sabre::Type::Entity Sabre::Builtins::Field::is_inf() { return TN::function(TN::boolean()); }

Sabre::Type::Entity Sabre::Builtins::Field::to_precision() { return to_scientific(); }
Sabre::Type::Entity Sabre::Builtins::Field::to_scientific() {
  return TN::function(TN::string(), TN::optional(TN::number()));
}

Sabre::Type::Entity Sabre::Builtins::Static::parse() {
  return TN::function(TN::number(), TN::any(), TN::optional(TN::number()));
}

template <>
Sabre::Type::Erased Sabre::Builtins::Apply<Sabre::Number::Tagged>::unary(const Type::Structure *, Operator::Kind kind) {
  switch (kind) {
  case Operator::Kind::INV: $_FALLTHROUGH;
  case Operator::Kind::NEG: return TN::number();
  default: return TN::unset(); // resolve now
  }
}

template <>
Sabre::Type::Erased Sabre::Builtins::Apply<Sabre::Number::Tagged>::binary(
    const Type::Structure *, Operator::Kind kind, const Type::Erased &right
) {
  // if we do not have a secondary "Number" typing, then ignore
  if (!right->is<Number::Tagged>()) return TN::unset();

  // attempt resolving a suitable kind to be used now
  switch (kind) {
  case Operator::Kind::ADD:
  case Operator::Kind::SUB: $_FALLTHROUGH;
  case Operator::Kind::MUL: $_FALLTHROUGH;
  case Operator::Kind::DIV: $_FALLTHROUGH;
  case Operator::Kind::MOD: $_FALLTHROUGH;
  case Operator::Kind::POW: $_FALLTHROUGH;

  case Operator::Kind::SHL: $_FALLTHROUGH;
  case Operator::Kind::SHR: $_FALLTHROUGH;
  case Operator::Kind::XOR: $_FALLTHROUGH;
  case Operator::Kind::BOR: $_FALLTHROUGH;
  case Operator::Kind::BAND: return TN::number();

  case Operator::Kind::LT: $_FALLTHROUGH;
  case Operator::Kind::GT: $_FALLTHROUGH;
  case Operator::Kind::LE: $_FALLTHROUGH;
  case Operator::Kind::GE: return TN::boolean();

  // otherwise use the default handler
  default: return TN::unset();
  }
}

//  PRIVATE METHODS  //

void Sabre::Builtins::Wrapper<Sabre::Number::Tagged>::m_typedefs(Type::World *globals) {
  // prepare the baseline details
  auto prototype = typeclass();
  auto &fields = prototype->fields();
  auto &statics = prototype->statics();

  // bind the required operators now
  prototype->operators() = Apply<Number::Tagged>::decide;

// define the fields for symbols
#define SABRE_XX_FIELDS_DEFINE(N, ...) fields.emplace(#N, Field::N());
#include "sabre/builtins/number/_defines/fields.def"

// define the statics for symbols
#define SABRE_XX_STATICS_DEFINE(N, ...) statics.emplace(#N, Static::N());
#include "sabre/builtins/number/_defines/statics.def"

  // define the baseline types
  globals->values().declare(name(), prototype);
  globals->types().declare(name(), TN::number());

  // and generate the "NaN" and "Infinity" typings as well
  globals->values().declare("NaN", TN::boolean());
  globals->values().declare("Infinity", TN::boolean());
}
