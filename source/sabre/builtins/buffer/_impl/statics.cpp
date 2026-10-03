/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  TYPEDEFS  //

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/buffer/_defines/statics.def"

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Static::empty(Isolate *isolate, const Args &) {
  return isolate->create<Iterable::Buffer>();
}

Sabre::Value::Any Sabre::Builtins::Static::filled(Isolate *isolate, const Args &args) {
  // ensure we have an incoming value
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 2);
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args[0]);
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args[1]);

  // prepare the incoming arguments now
  auto size = args.at<Number::Tagged>(0).value();
  SABRE_MM_ASSERT_LOWER(isolate, size, 0);
  SABRE_MM_ASSERT_INTEGRAL(isolate, size);

  // ensure that our fill value is valid as well
  auto value = args.at<Number::Tagged>(1).value();
  SABRE_MM_ASSERT_INTEGRAL(isolate, size);
  SABRE_MM_ASSERT_INDEX(isolate, value, 0xFF);

  // prepare the list and set all values as needed
  auto list = isolate->create<Iterable::Buffer>(size);
  std::ranges::fill_n(list.data(), size, value);

  // return the resulting list now
  return list;
}

//  PRIVATE METHODS  //

Sabre::Value::Any
Sabre::Builtins::Wrapper<Sabre::Iterable::Buffer>::m_globals(Isolate *isolate, const Object::Class &self) {
#define SABRE_XX_STATICS_DEFINE(N, ...)                                                \
  self.statics().emplace(#N, Member::Factory::native(isolate, Static::N, name(), #N));
#include "sabre/builtins/buffer/_defines/statics.def"

  // and return the resulting instance
  return self;
}
