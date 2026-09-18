/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  TYPEDEFS  //

#define SABRE_XX_FIELDS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Field, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/iterator/_defines/fields.def"

//  PROPERTIES  //

static auto s_members = Sabre::Builtins::Storage<Sabre::Iterable::Iterator>({
#define SABRE_XX_FIELDS_DEFINE(N, ...) {#N, Sabre::Builtins::Field::N},
#include "sabre/builtins/iterator/_defines/fields.def"
});

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Field::done(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Iterable::Iterator, args.self());
  return Value::Boolean(args.self<Iterable::Iterator>().done());
}

Sabre::Value::Any Sabre::Builtins::Field::next(Isolate *isolate, const Args &args) {
  // validate the incoming iterator now
  SABRE_MM_ASSERT_TYPEOF(isolate, Iterable::Iterator, args.self());

  // pull out our necessary details
  auto iterator = args.self<Iterable::Iterator>();
  Value::Any result = Value::Boolean(iterator.next(isolate));

  // and validate the result now
  return iterator.okay() ? result : Value::Failure();
}

Sabre::Value::Any Sabre::Builtins::Field::index(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Iterable::Iterator, args.self());
  return Number::Tagged(args.self<Iterable::Iterator>().index());
}

Sabre::Value::Any Sabre::Builtins::Field::collect(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Iterable::Iterator, args.self());
  return isolate->create<Iterable::List>(args.self<Iterable::Iterator>());
}

//  PRIVATE METHODS  //

Sabre::Member::View Sabre::Builtins::Wrapper<Sabre::Iterable::Iterator>::m_attribute(
    const Iterable::Iterator &, const Value::Symbol &symbol
) {
  return s_members.retrieve(symbol);
}
