/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

/// Forward Declarations
$_FWD(Sabre::Builtins::Detail, using Callback = Value::Any (*)(const Object::Variant *))
$_FWD(Sabre::Builtins::Static, static Value::Any resolve(Isolate *isolate, const Args &args, Detail::Callback callback))

//  TYPEDEFS  //

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/enum/_defines/statics.def"

//  PUBLIC METHODS  //

static Sabre::Value::Any
Sabre::Builtins::Static::resolve(Isolate *isolate, const Args &args, Detail::Callback callback) {
  // validate some details about the incoming items
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 2);
  SABRE_MM_ASSERT_TYPEOF(isolate, Object::Enum, args[0]);
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args[1]);

  // pull out the value in question to be resolved
  auto ordinal = args.at<Number::Tagged>(1);
  auto enumeration = args.at<Object::Enum>(0);
  auto variant = enumeration.resolve(ordinal);
  return variant ? callback(variant) : isolate->panic(4000500, ordinal);
}

Sabre::Value::Any Sabre::Builtins::Static::name(Isolate *isolate, const Args &args) {
  return resolve(isolate, args, [](const Object::Variant *variant) -> Value::Any { return variant->name; });
}

Sabre::Value::Any Sabre::Builtins::Static::label(Isolate *isolate, const Args &args) {
  return resolve(isolate, args, [](const Object::Variant *variant) -> Value::Any { return variant->label; });
}

Sabre::Value::Any Sabre::Builtins::Static::value(Isolate *isolate, const Args &args) {
  return resolve(isolate, args, [](const Object::Variant *variant) { return variant->value->reference(); });
}

//  PRIVATE METHODS  //

Sabre::Value::Any
Sabre::Builtins::Wrapper<Sabre::Object::Enum>::m_globals(Isolate *isolate, const Object::Class &self) {
#define SABRE_XX_STATICS_DEFINE(N, ...)                                                \
  self.statics().emplace(#N, Member::Factory::native(isolate, Static::N, name(), #N));
#include "sabre/builtins/enum/_defines/statics.def"

  // and return the resulting instance
  return self;
}
