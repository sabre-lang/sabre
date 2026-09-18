/// Sabre Modules
#include "sabre/string/literal.hpp"
#include "sabre/function/args.hpp"
#include "sabre/number/tagged.hpp"
#include "sabre/runtime/isolate.hpp"

/// Forward Declarations
$_FWD(Sabre::String::Literal, void push(Args &, const Value::Any &))
$_FWD(Sabre::String::Literal, void push(std::stringstream &, Args &, const Value::Any &))
$_FWD(Sabre::String::Literal, Value::Any report(Runtime::Isolate *, const std::exception &))
$_FWD(Sabre::String::Literal, Value::Any report(Runtime::Isolate *, const $::String::View &))

//  PUBLIC METHODS  //

void Sabre::String::Literal::push(Args &store, const Value::Any &value) {
  std::stringstream oss = {};
  push(oss, store, value);
}

void Sabre::String::Literal::push(std::stringstream &oss, Args &store, const Value::Any &value) {
  if (value.is<Number::Tagged>()) store.push_back(value.as<Number::Tagged>().value());
  else oss << value, store.push_back(oss.str()), std::stringstream().swap(oss);
}

Sabre::Value::Any Sabre::String::Literal::style(Runtime::Isolate *isolate, const Value::Any &value) {
  return style(isolate, "{0}", value);
}

Sabre::Value::Any
Sabre::String::Literal::style(Runtime::Isolate *isolate, const String::Any &message, const Value::Any &value) {
  return style(isolate, message.view(), value);
}

Sabre::Value::Any
Sabre::String::Literal::style(Runtime::Isolate *isolate, const $::String::View &message, const Value::Any &value) {
  // fast-path where a string is already valid
  if (value.is<String::Any>()) return value;

  // and construct the args to be used
  auto store = Args();
  push(store, value);

  // finally finish formatting the instance
  return style(isolate, message, std::move(store));
}

Sabre::Value::Any
Sabre::String::Literal::style(Runtime::Isolate *isolate, const String::Any &message, const Function::Args &args) {
  return style(isolate, message.view(), args);
}

Sabre::Value::Any
Sabre::String::Literal::style(Runtime::Isolate *isolate, const $::String::View &message, const Function::Args &args) {
  // prepare a string-stream for conversion
  auto oss = std::stringstream();

  // and construct the args to be used
  auto store = Args();
  store.reserve(args.size(), 0);

  // emplace each of the store values now
  for (const auto &value : args.span()) push(oss, store, value);

  // and finally request styling now
  return style(isolate, message, std::move(store));
}

Sabre::Value::Any Sabre::String::Literal::style(Runtime::Isolate *isolate, const String::Any &message, Args &&args) {
  return style(isolate, message.view(), std::move(args));
}

Sabre::Value::Any
Sabre::String::Literal::style(Runtime::Isolate *isolate, const $::String::View &message, Args &&args) {
  // clang-format off
  try { return Any(isolate, fmt::vformat(message, std::move(args))); }
  catch (const std::exception &exception) { return report(isolate, exception); }
  // clang-format on
}

Sabre::Value::Any Sabre::String::Literal::report(Runtime::Isolate *isolate, const std::exception &exception) {
  return report(isolate, exception.what());
}

Sabre::Value::Any Sabre::String::Literal::report(Runtime::Isolate *isolate, const $::String::View &message) {
  return isolate->panic(6000403, $::Convert::capitalize(message));
}
