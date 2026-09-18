/// Sabre Includes
#include "sabre/function/args.hpp"
#include "sabre/runtime/isolate.hpp"

/// Value Includes
#include "sabre/value/_inline/value.ipp"

//  MACROS  //

/// @brief Prepare a macro for testing our values
#define MM_ASSERT_NUMERIC(I, V, ...)                                                                                 \
  if (!(V).is<Number::Tagged>()) return I->panic(6000253, #V, Value::Inspect<Number::Tagged>::name()), std::nullopt;

//  PUBLIC METHODS  //

std::optional<Sabre::Iterable::Slice>
Sabre::Iterable::Deduce::slice(Runtime::Isolate *isolate, const Function::Args &args, int64_t size) {
  // otherwise attempt pulling out our sections to be erased
  auto initial = args.at(0, Value::Void());
  auto secondary = args.at(1, Value::Void());

  // update our values with the correct details now
  if (initial.is<Value::Void>()) initial = Number::Tagged(0);
  if (secondary.is<Value::Void>()) secondary = Number::Tagged(size);

  // prepare our starting and ending points now
  MM_ASSERT_NUMERIC(isolate, initial);
  MM_ASSERT_NUMERIC(isolate, secondary);

  // cast the incoming values now
  Number::Integral start = initial.as<Number::Tagged>();
  Number::Integral end = secondary.as<Number::Tagged>();

  // if the arguments are less than zero
  if (start < 0) start += size;
  if (end < 0) end += size;

  // if our values are equal, then stop
  if (start == end) return Slice();

  // validate the incoming values now
  start = std::clamp(start, Number::Integral(0), size);
  end = std::clamp(end, Number::Integral(0), size);

  // swap our values to ensure correct
  if (start > end) std::swap(start, end);

  // should be able to return the expected slice
  return Slice(start, end);
}

std::optional<Sabre::Iterable::Interval>
Sabre::Iterable::Deduce::interval(Runtime::Isolate *isolate, const Function::Args &args) {
  // prepare a suitable selection of values to be used
  Number::Floating start = 0, stop = 0, step = 1;

  switch (auto size = args.size(); size) {
  // leave all values as the same here
  case 0: break;

  case 1: {
    MM_ASSERT_NUMERIC(isolate, args[0]);
    stop = args.at<Number::Tagged>(0).value();
    if (start > stop) step = -1; // invert step
  } break;

  default: MM_ASSERT_NUMERIC(isolate, args[2]); $_FALLTHROUGH;
  case 2: {
    MM_ASSERT_NUMERIC(isolate, args[0]);
    MM_ASSERT_NUMERIC(isolate, args[1]);
    start = args.at<Number::Tagged>(0).value();
    stop = args.at<Number::Tagged>(1).value();
    if (size == 2) step = start < stop ? 1 : -1;
    else step = args.at<Number::Tagged>(2).value();
  } break;
  }

  // construct the resulting interval now
  return Interval(start, stop, step);
}
