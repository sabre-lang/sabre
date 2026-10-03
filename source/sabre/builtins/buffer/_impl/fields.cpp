/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  TYPEDEFS  //

#define SABRE_XX_FIELDS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Field, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/buffer/_defines/fields.def"

//  PROPERTIES  //

static auto s_members = Sabre::Builtins::Storage<Sabre::Iterable::Buffer>({
#define SABRE_XX_FIELDS_DEFINE(N, ...) {#N, Sabre::Builtins::Field::N},
#include "sabre/builtins/buffer/_defines/fields.def"
});

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Field::size(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Iterable::Buffer, args.self());
  return Number::Tagged(args.self<Iterable::Buffer>().size());
}

Sabre::Value::Any Sabre::Builtins::Field::empty(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Iterable::Buffer, args.self());
  return Value::Boolean(args.self<Iterable::Buffer>().empty());
}

Sabre::Value::Any Sabre::Builtins::Field::front(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Iterable::Buffer, args.self());
  auto self = args.self<Iterable::Buffer>(); // get the buffer
  if (self.empty()) return isolate->panic(6000504, "front");
  return Number::Tagged(self.front()); // can validly get
}

Sabre::Value::Any Sabre::Builtins::Field::back(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Iterable::Buffer, args.self());
  auto self = args.self<Iterable::Buffer>(); // get the buffer
  if (self.empty()) return isolate->panic(6000504, "back");
  return Number::Tagged(self.back()); // can validly get
}

Sabre::Value::Any Sabre::Builtins::Field::map(Isolate *isolate, const Args &args) {
  // ensure some conditions about the list
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, Function::Any, args[0]);
  SABRE_MM_ASSERT_TYPEOF(isolate, Iterable::Buffer, args.self());

  // pull out our details now for handling
  auto values = args.self<Iterable::Buffer>().span();
  auto callback = args.at<Function::Any>(0);

  // prepare the outgoing arguments to be used
  auto passthrough = std::vector<Value::Any>(2);

  // prepare the filtered view of values now
  auto mapped = std::vector<uint8_t>(values.size());

  // iterate over the available values now
  for (size_t ii = 0; ii < values.size(); ++ii) {
    // prepare the outgoing passthrough arguments now
    passthrough[0] = Number::Tagged(values[ii]);
    passthrough[1] = Number::Tagged(ii);

    // attempt executing our result now
    auto result = isolate->invoke(callback, std::span(passthrough));

    // if the result failed, then pass onwards
    if (!result.pointer().okay()) return result;
    else if (result.is<Number::Tagged>()) mapped[ii] = result.as<Number::Tagged>().value();
    else return isolate->panic(6000253, "result", Value::Inspect<Number::Tagged>::name());
  }

  // construct the filtered list now
  return isolate->create<Iterable::Buffer>(mapped);
}

Sabre::Value::Any Sabre::Builtins::Field::fold(Isolate *isolate, const Args &args) {
  // ensure some conditions about the list
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 2);
  SABRE_MM_ASSERT_TYPEOF(isolate, Function::Any, args[1]);
  SABRE_MM_ASSERT_TYPEOF(isolate, Iterable::Buffer, args.self());

  // pull out our details now for handling
  auto result = args.at<Value::Any>(0);
  auto callback = args.at<Function::Any>(1);
  auto self = args.self<Iterable::Buffer>();

  // prepare the arguments to be passed on
  std::vector<Value::Any> passthrough = {result, Value::Void()};

  // iterate over the available values now
  for (const auto &value : self.span()) {
    passthrough[0] = result, passthrough[1] = Number::Tagged(value);
    result = isolate->invoke(callback, {passthrough});
    if (!result.pointer().okay()) return result;
  }

  // construct the filtered list now
  return result;
}

Sabre::Value::Any Sabre::Builtins::Field::slice(Isolate *isolate, const Args &args) {
  // ensure some conditions about the function call
  SABRE_MM_ASSERT_TYPEOF(isolate, Iterable::Buffer, args.self());

  // pull out the incoming list now
  auto self = args.self<Iterable::Buffer>();
  auto size = static_cast<int64_t>(self.size());

  // if no arguments are given, then clear the list
  if (args.empty()) return isolate->create<Iterable::Buffer>(self.span());

  // otherwise pull out our arguments needed
  auto slice = Iterable::Deduce::slice(isolate, args, size);
  if (!slice.has_value()) return Value::Failure();

  // resolve the base places to be used
  auto start = slice->start(), stop = slice->stop();

  // and finally construct the resulting slice now
  return isolate->create<Iterable::Buffer>(self.slice(start, stop - start));
}

Sabre::Value::Any Sabre::Builtins::Field::reverse(Isolate *isolate, const Args &args) {
  // ensure some conditions about the list
  SABRE_MM_ASSERT_TYPEOF(isolate, Iterable::Buffer, args.self());

  // prepare the values to be reversed now
  auto self = args.self<Iterable::Buffer>();
  auto reversed = self.span() | std::views::reverse;

  // and construct the resulting array now
  return isolate->create<Iterable::Buffer>($::Ranges::To(reversed));
}

Sabre::Value::Any Sabre::Builtins::Field::first_index_of(Isolate *isolate, const Args &args) {
  // ensure some conditions about the list
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args[0]);
  SABRE_MM_ASSERT_TYPEOF(isolate, Iterable::Buffer, args.self());

  // pull out the incoming values now
  auto self = args.self<Iterable::Buffer>();

  // prepare our search items now
  auto haystack = self.span();
  auto needle = args.at<Number::Tagged>(0).value();

  // ensure our needle can be suitable used as a byte
  SABRE_MM_ASSERT_INTEGRAL(isolate, needle);
  SABRE_MM_ASSERT_INDEX(isolate, needle, 0xFF);

  // attempt finding the needle in the haystack
  auto iter = std::ranges::find(haystack, needle);

  // should be able to safely resolve the index of the value
  if (iter == haystack.end()) return Number::Tagged(-1);
  return Number::Tagged(std::distance(haystack.begin(), iter));
}

Sabre::Value::Any Sabre::Builtins::Field::last_index_of(Isolate *isolate, const Args &args) {
  // ensure some conditions about the list
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, Iterable::Buffer, args.self());

  // pull out the incoming values now
  auto self = args.self<Iterable::Buffer>();

  // prepare our search items now
  auto haystack = self.span();
  auto needle = args.at<Number::Tagged>(0).value();

  // ensure our needle can be suitable used as a byte
  SABRE_MM_ASSERT_INTEGRAL(isolate, needle);
  SABRE_MM_ASSERT_INDEX(isolate, needle, 0xFF);

  // attempt finding the needle in the haystack
  auto iter = std::ranges::find(haystack | std::views::reverse, needle);

  // should be able to safely resolve the index of the value
  if (iter == haystack.rend()) return Number::Tagged(-1);
  return Number::Tagged(std::distance(iter, haystack.rend()) - 1);
}

//  PRIVATE METHODS  //

Sabre::Member::View
Sabre::Builtins::Wrapper<Sabre::Iterable::Buffer>::m_attribute(const Iterable::Buffer &, const Value::Symbol &symbol) {
  return s_members.retrieve(symbol);
}
