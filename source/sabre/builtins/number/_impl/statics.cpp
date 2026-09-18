/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  TYPEDEFS  //

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/number/_defines/statics.def"

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Static::parse(Isolate *isolate, const Args &args) {
  // ensure the total incoming arguments now
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);

  // pull out the necessary properties now
  auto input = args.at<String::Any>(0);
  auto radix = args.at(1, Value::Void());

  // attempt via a variety of options
  try {
    // if we have no-radix given (eg: undefined base), then revert to float
    if (radix.is<Value::Void>()) return Number::Tagged(std::stod(input.data()));

    // otherwise ensure we have a valid radix incoming
    SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, radix);

    // pull out the radix value to be used (eg: as a base)
    Number::Floating base = radix.as<Number::Tagged>();

    // and attempt conversion now using integral parsing
    return Number::Tagged(std::stoll(input.data(), nullptr, base));
  }

  // otherwise default exceptions to be "NAN"
  catch (...) {
    return Number::Invalid;
  }
}

//  PRIVATE METHODS  //

Sabre::Value::Any
Sabre::Builtins::Wrapper<Sabre::Number::Tagged>::m_globals(Isolate *isolate, const Object::Class &self) {
#define SABRE_XX_STATICS_DEFINE(N, ...)                                                \
  self.statics().emplace(#N, Member::Factory::native(isolate, Static::N, name(), #N));
#include "sabre/builtins/number/_defines/statics.def"

  // and return the resulting instance
  return self;
}
