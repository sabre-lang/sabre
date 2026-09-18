/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  TYPEDEFS  //

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/symbol/_defines/statics.def"

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Static::from(Isolate *isolate, const Args &args) {
  // ensure the correct number of arguments given
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);

  // get the value to question and validate as needed
  auto value = args.at(0);

  // cast depending on the type of the value
  if (value.is<String::Any>()) return value.as<String::Any>().symbol();

  /// TODO: define a better hasher for other values
  return Value::Symbol();
}

//  PRIVATE METHODS  //

Sabre::Value::Any
Sabre::Builtins::Wrapper<Sabre::Value::Symbol>::m_globals(Isolate *isolate, const Object::Class &self) {
#define SABRE_XX_STATICS_DEFINE(N, ...)                                                \
  self.statics().emplace(#N, Member::Factory::native(isolate, Static::N, name(), #N));
#include "sabre/builtins/symbol/_defines/statics.def"

  // and return the resulting instance
  return self;
}
