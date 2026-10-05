/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

/// Forward Declarations
$_FWD(Sabre::Builtins::Apply, void append(std::vector<uint8_t> &, const Value::Any &))

//  TYPEDEFS  //

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/buffer/_defines/statics.def"

void Sabre::Builtins::Apply::append(std::vector<uint8_t> &bytes, const Value::Any &value) {
  if (!value.is<Number::Tagged>()) bytes.emplace_back(value.truthiness());
  else bytes.emplace_back(value.as<Number::Tagged>().value());
}

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Static::from(Isolate *isolate, const Args &args) {
  // ensure we have an incoming value now
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);

  // if we have an input array, then cache the size
  auto *list = args.when<Iterable::List>(0);
  auto size = list ? list->size() : 0;

  // attempt resolving the base iterable to be used
  auto value = Iterable::Resolve(isolate, args[0]);

  // handle based on whether the iterator is okay or now
  if (!value.pointer().okay()) return isolate->panic(6000502, value.brand());

  // we need to suitably resolve the iterator now
  auto iterator = value.as<Iterable::Iterator>();

  // prepare the temporary data to be used now
  auto bytes = std::vector<uint8_t>();
  bytes.reserve(size); // bind sizing

  // and iterate over the available items now
  while (!iterator.next(isolate)) Apply::append(bytes, iterator.value());

  // construct the resulting buffer now
  return isolate->create<Iterable::Buffer>(std::move(bytes));
}

Sabre::Value::Any Sabre::Builtins::Static::encode(Isolate *isolate, const Args &args) {
  // ensure we have an incoming value now
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);

  // if we have been given a string, fast path
  auto text = args.at<String::Any>(0);

  // construct our buffer from the given text
  return isolate->create<Iterable::Buffer>(text.view());
}

Sabre::Value::Any Sabre::Builtins::Static::decode(Isolate *isolate, const Args &args) {
  // ensure we have an incoming value now
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, Iterable::Buffer, args[0]);

  // if we have been given a string, fast path
  auto buffer = args.at<Iterable::Buffer>(0);

  // construct our buffer from the given text
  return String::Any(isolate, buffer.view());
}

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

  // prepare the list and set all values as needed
  auto list = isolate->create<Iterable::Buffer>(size);
  std::ranges::fill_n(list.data(), size, static_cast<uint8_t>(value));

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
