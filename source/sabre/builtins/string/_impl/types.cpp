/// Sabre Includes
#include "sabre/string/common.hpp"

/// Builtin Includes
#include "sabre/builtins/_inline/apply.ipp"
#include "sabre/builtins/_inline/builtins.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Definitions
$_FWD(Sabre::Builtins, using TN = Type::New)

//  TYPEDEFS  //

#define SABRE_XX_FIELDS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Field, static Type::Entity N())
#include "sabre/builtins/string/_defines/fields.def"

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Type::Entity N())
#include "sabre/builtins/string/_defines/statics.def"

//  PUBLIC METHODS  //

$::Shared::Pointer<Sabre::Type::Prototype> Sabre::Builtins::Wrapper<Sabre::String::Any>::typeclass() {
  return m_typeclass([](const auto &) {});
}

Sabre::Type::Entity Sabre::Builtins::Field::size() { return TN::function(TN::number()); }
Sabre::Type::Entity Sabre::Builtins::Field::bytes() { return TN::function(TN::number()); }
Sabre::Type::Entity Sabre::Builtins::Field::empty() { return TN::function(TN::boolean()); }
Sabre::Type::Entity Sabre::Builtins::Field::symbol() { return TN::function(TN::symbol()); }
Sabre::Type::Entity Sabre::Builtins::Field::fmt() { return TN::variadic(TN::string()); }

Sabre::Type::Entity Sabre::Builtins::Field::at() { return TN::function(TN::maybe(TN::string()), TN::number()); }

Sabre::Type::Entity Sabre::Builtins::Field::slice() {
  auto index = TN::optional(TN::number());
  return TN::function(TN::string(), index, index);
}

Sabre::Type::Entity Sabre::Builtins::Field::compare() { return TN::function(TN::number(), TN::string()); }
Sabre::Type::Entity Sabre::Builtins::Field::contains() {
  return TN::function(TN::boolean(), TN::string(), TN::optional(TN::number()));
}

Sabre::Type::Entity Sabre::Builtins::Field::starts_with() { return ends_with(); }
Sabre::Type::Entity Sabre::Builtins::Field::ends_with() { return TN::function(TN::boolean(), TN::string()); }

Sabre::Type::Entity Sabre::Builtins::Field::to_lower() { return TN::function(TN::string()); }
Sabre::Type::Entity Sabre::Builtins::Field::to_upper() { return TN::function(TN::string()); }

Sabre::Type::Entity Sabre::Builtins::Field::trim_both() { return TN::function(TN::string()); }
Sabre::Type::Entity Sabre::Builtins::Field::trim_leading() { return TN::function(TN::string()); }
Sabre::Type::Entity Sabre::Builtins::Field::trim_trailing() { return TN::function(TN::string()); }

Sabre::Type::Entity Sabre::Builtins::Field::pad_leading() { return pad_trailing(); }
Sabre::Type::Entity Sabre::Builtins::Field::pad_trailing() {
  return TN::function(TN::string(), TN::number(), TN::optional(TN::string()));
}

Sabre::Type::Entity Sabre::Builtins::Static::from() { return TN::function(TN::string(), TN::any()); }
Sabre::Type::Entity Sabre::Builtins::Static::codepoint() { return TN::function(TN::string(), TN::number()); }

template <>
Sabre::Type::Erased Sabre::Builtins::Apply<Sabre::String::Any>::unary(const Type::Structure *, Operator::Kind kind) {
  switch (kind) {
  case Operator::Kind::ITER: return TN::string();
  default: return TN::unset(); // resolve now
  }
}

template <>
Sabre::Type::Erased Sabre::Builtins::Apply<Sabre::String::Any>::binary(
    const Type::Structure *, Operator::Kind kind, const Type::Erased &right
) {
  // if we do not have a secondary "String" typing, then ignore
  if (!right->is<String::Any>()) return TN::unset();

  // attempt matching as necessary now
  switch (kind) {
  // check against the basic addition handler
  case Operator::Kind::ADD: return TN::string();

  // allow checking against comparison handlers
  case Operator::Kind::LT: $_FALLTHROUGH;
  case Operator::Kind::GT: $_FALLTHROUGH;
  case Operator::Kind::LE: $_FALLTHROUGH;
  case Operator::Kind::GE: return TN::boolean();

  // fallback to the default handler
  default: return TN::unset();
  }
}

//  PRIVATE METHODS  //

void Sabre::Builtins::Wrapper<Sabre::String::Any>::m_typedefs(Type::World *globals) {
  // prepare the baseline details
  auto prototype = typeclass();
  auto &fields = prototype->fields();
  auto &statics = prototype->statics();

  // bind the decision tree for operators
  prototype->operators() = Apply<String::Any>::decide;

// define the fields for symbols
#define SABRE_XX_FIELDS_DEFINE(N, ...) fields.emplace(#N, Field::N());
#include "sabre/builtins/string/_defines/fields.def"

// define the statics for symbols
#define SABRE_XX_STATICS_DEFINE(N, ...) statics.emplace(#N, Static::N());
#include "sabre/builtins/string/_defines/statics.def"

  // define the baseline types
  globals->values().declare(name(), prototype);
  globals->types().declare(name(), TN::string());
}
