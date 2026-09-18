/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  TYPEDEFS  //

#define SABRE_XX_FIELDS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Field, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/function/_defines/fields.def"

//  PROPERTIES  //

static auto s_members = Sabre::Builtins::Storage<Sabre::Function::Any>({
#define SABRE_XX_FIELDS_DEFINE(N, ...) {#N, Sabre::Builtins::Field::N},
#include "sabre/builtins/function/_defines/fields.def"
});

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Field::arity(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Function::Any, args.self());
  return Number::Tagged(args.self<Function::Any>().arity());
}

Sabre::Value::Any Sabre::Builtins::Field::adicity(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Function::Any, args.self());
  return Number::Tagged(args.self<Function::Any>().adicity());
}

Sabre::Value::Any Sabre::Builtins::Field::receiver(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Function::Any, args.self());
  return args.self<Function::Any>().receiver(); // get now
}

//  PRIVATE METHODS  //

Sabre::Member::View
Sabre::Builtins::Wrapper<Sabre::Function::Any>::m_attribute(const Function::Any &, const Value::Symbol &symbol) {
  return s_members.retrieve(symbol);
}
