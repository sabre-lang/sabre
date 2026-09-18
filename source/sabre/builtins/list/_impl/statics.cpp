/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  TYPEDEFS  //

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/list/_defines/statics.def"

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Static::from(Isolate *isolate, const Args &args) {
  // ensure we have an incoming value
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);

  // pull out the incoming iterable value now
  auto value = args[0];

  // fast-path iterator values if given at all
  if (value.is<Iterable::Iterator>()) return isolate->create<Iterable::List>(value.as<Iterable::Iterator>());

  // prepare the incoming descriptor to be used
  auto *descriptor = value.attribute(Operator::Kind::ITER);

  // attempt getting the iterable to be used
  auto attribute = descriptor ? descriptor->getter(isolate, value) : Value::Missing();
  if (!attribute.pointer().okay()) return isolate->panic(6000502, value.brand());

  // should be able to cast and convert to a suitable array now
  return isolate->create<Iterable::List>(attribute.as<Iterable::Iterator>());
}

Sabre::Value::Any Sabre::Builtins::Static::empty(Isolate *isolate, const Args &) {
  return isolate->create<Iterable::List>();
}

Sabre::Value::Any Sabre::Builtins::Static::range(Isolate *isolate, const Args &args) {
  // prepare the basic interval details
  auto interval = Iterable::Deduce::interval(isolate, args);

  // ensure some certain conditions about the interval
  if (!interval.has_value()) return Value::Failure();
  else if (!interval->finite()) return isolate->panic(6000603);

  // prepare the iteration values to use now
  auto vs = interval->start(), ve = interval->stop(), vx = interval->step();

  // attempt constructing the expected size to be used
  int32_t size = std::abs(vs - ve) / std::abs(vx);

  // construct the resulting set of integers and numerics required now
  auto callback = [vs, vx](int32_t ii) -> Value::Any { return Number::Tagged(vs + (ii * vx)); };
  auto range = $::Ranges::To(std::views::iota(0, size) | std::views::transform(callback));

  // construct the resulting list instance now
  return isolate->create<Iterable::List>(range);
}

Sabre::Value::Any Sabre::Builtins::Static::filled(Isolate *isolate, const Args &args) {
  // ensure we have an incoming value
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 2);
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args[0]);

  // prepare the incoming arguments now
  auto size = args.at<Number::Tagged>(0).value();
  SABRE_MM_ASSERT_LOWER(isolate, size, 0);
  SABRE_MM_ASSERT_INTEGRAL(isolate, size);

  // prepare the list and set all values as needed
  auto list = isolate->create<Iterable::List>(size);
  std::ranges::fill_n(list.data(), size, args[1]);

  // return the resulting list now
  return list;
}

//  PRIVATE METHODS  //

Sabre::Value::Any
Sabre::Builtins::Wrapper<Sabre::Iterable::List>::m_globals(Isolate *isolate, const Object::Class &self) {
#define SABRE_XX_STATICS_DEFINE(N, ...)                                                \
  self.statics().emplace(#N, Member::Factory::native(isolate, Static::N, name(), #N));
#include "sabre/builtins/list/_defines/statics.def"

  // and return the resulting instance
  return self;
}
