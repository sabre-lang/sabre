/// Sabre Includes
#include "sabre/member/factory.hpp"
#include "sabre/runtime/isolate.hpp"

/// Builtin Modules
#include "sabre/builtins/_inline/builtins.ipp"

//  TYPEDEFS   //

#define SABRE_XX_FIELDS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Field, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/debug/_defines/fields.def"

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Field::print(Isolate *, const Args &args) {
  return $::Debug::print("{0}", fmt::join(args.span(), " ")), Value::Void();
}

Sabre::Value::Any Sabre::Builtins::Field::eprint(Isolate *, const Args &args) {
  return $::Debug::eprint("{0}", fmt::join(args.span(), " ")), Value::Void();
}

Sabre::Value::Any Sabre::Builtins::Field::println(Isolate *, const Args &args) {
  return $::Debug::println("{0}", fmt::join(args.span(), " ")), Value::Void();
}

Sabre::Value::Any Sabre::Builtins::Field::eprintln(Isolate *, const Args &args) {
  return $::Debug::eprintln("{0}", fmt::join(args.span(), " ")), Value::Void();
}

Sabre::Value::Any Sabre::Builtins::Field::breakpoint(Isolate *isolate, const Args &) {
  return isolate->todo("Unimplemented 'Debug.breakpoint'");
}

//  PRIVATE METHODS  //

Sabre::Value::Any Sabre::Builtins::Wrapper<Sabre::Builtins::Custom::Debug>::m_globals(Isolate *isolate) {
  // construct the base object instance
  auto self = isolate->create<Object::Instance>();

  // assign all the available fields to be used
#define SABRE_XX_FIELDS_DEFINE(N, ...)                                               \
  self.fields().emplace(#N, Member::Factory::native(isolate, Field::N, name(), #N));
#include "sabre/builtins/debug/_defines/fields.def"

  // and return the resulting instance
  return self;
}
