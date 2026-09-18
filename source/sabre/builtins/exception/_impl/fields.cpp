/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  TYPEDEFS  //

#define SABRE_XX_FIELDS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Field, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/exception/_defines/fields.def"

//  PROPERTIES  //

static auto s_members = Sabre::Builtins::Storage<Sabre::Object::Exception>({
#define SABRE_XX_FIELDS_DEFINE(N, ...) {#N, Sabre::Builtins::Field::N},
#include "sabre/builtins/exception/_defines/fields.def"
});

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Field::name(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Object::Exception, args.self());
  return args.self<Object::Exception>().name(); // get the name
}

Sabre::Value::Any Sabre::Builtins::Field::message(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Object::Exception, args.self());
  return args.self<Object::Exception>().message(); // get the name
}

//  PRIVATE METHODS  //

Sabre::Member::View Sabre::Builtins::Wrapper<Sabre::Object::Exception>::m_attribute(
    const Object::Exception &, const Value::Symbol &symbol
) {
  return s_members.retrieve(symbol);
}
