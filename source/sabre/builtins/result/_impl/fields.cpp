/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  TYPEDEFS  //

#define SABRE_XX_FIELDS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Field, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/result/_defines/fields.def"

//  PROPERTIES  //

static auto s_members = Sabre::Builtins::Storage<Sabre::Monad::Result>({
#define SABRE_XX_FIELDS_DEFINE(N, ...) {#N, Sabre::Builtins::Field::N},
#include "sabre/builtins/result/_defines/fields.def"
});

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Field::is_okay(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Monad::Result, args.self());
  return Value::Boolean(args.self<Monad::Result>().success());
}

Sabre::Value::Any Sabre::Builtins::Field::is_error(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Monad::Result, args.self());
  return Value::Boolean(args.self<Monad::Result>().failure());
}

Sabre::Value::Any Sabre::Builtins::Field::unwrap_okay(Isolate *isolate, const Args &args) {
  // attempt parsing the incoming value
  SABRE_MM_ASSERT_TYPEOF(isolate, Monad::Result, args.self());

  // get the baseline result value
  auto result = args.self<Monad::Result>();

  // succeed immediately if the result is okay
  if (result.success()) return result.storage();

  // otherwise throw an error as needed
  if (args.empty()) return isolate->panic(6000701);
  else return isolate->panic(6000700, args[1]);
}

Sabre::Value::Any Sabre::Builtins::Field::unwrap_error(Isolate *isolate, const Args &args) {
  // attempt parsing the incoming value
  SABRE_MM_ASSERT_TYPEOF(isolate, Monad::Result, args.self());

  // get the baseline result value
  auto result = args.self<Monad::Result>();

  // succeed immediately if the result is an error
  if (result.failure()) return result.storage();

  // otherwise throw an error as needed
  if (args.empty()) return isolate->panic(6000702);
  else return isolate->panic(6000700, args[1]);
}

//  PRIVATE METHODS  //

Sabre::Member::View
Sabre::Builtins::Wrapper<Sabre::Monad::Result>::m_attribute(const Monad::Result &, const Value::Symbol &symbol) {
  return s_members.retrieve(symbol);
}
