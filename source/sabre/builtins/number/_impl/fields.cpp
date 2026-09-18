/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

/// Forward Declarations
$_FWD(Sabre::Builtins::Field, static Value::Any stringify(Isolate *, const Args &, const $::String::View &))

//  TYPEDEFS  //

#define SABRE_XX_FIELDS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Field, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/number/_defines/fields.def"

//  PROPERTIES  //

static auto s_members = Sabre::Builtins::Storage<Sabre::Number::Tagged>({
#define SABRE_XX_FIELDS_DEFINE(N, ...) {#N, Sabre::Builtins::Field::N},
#include "sabre/builtins/number/_defines/fields.def"
});

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Field::abs(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args.self());
  return Number::Tagged(std::abs(args.self<Number::Tagged>().value()));
}

Sabre::Value::Any Sabre::Builtins::Field::sign(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args.self());
  auto value = args.self<Number::Tagged>().value();   // get value...
  return Number::Tagged(Number::Math::signum(value)); // ...and sign
}

Sabre::Value::Any Sabre::Builtins::Field::ceil(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args.self());
  return Number::Tagged(std::ceil(args.self<Number::Tagged>().value()));
}

Sabre::Value::Any Sabre::Builtins::Field::floor(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args.self());
  return Number::Tagged(std::floor(args.self<Number::Tagged>().value()));
}

Sabre::Value::Any Sabre::Builtins::Field::round(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args.self());
  return Number::Tagged(std::round(args.self<Number::Tagged>().value()));
}

Sabre::Value::Any Sabre::Builtins::Field::trunc(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args.self());
  return Number::Tagged(std::trunc(args.self<Number::Tagged>().value()));
}

Sabre::Value::Any Sabre::Builtins::Field::clamp(Isolate *isolate, const Args &args) {
  // ensure our self value given is a valid number
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args.self());

  // get the baseline numeric to be used
  auto self = args.self<Number::Tagged>();

  // prepare the minimum and maximum values
  auto minimum = args.at(0, self), maximum = args.at(1, self);

  // update our values to be non-voidish now
  if (minimum.is<Value::Void>()) minimum = self;
  if (maximum.is<Value::Void>()) maximum = self;

  // ensure our values are now numerics
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, minimum);
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, maximum);

  // get the underlying left/right clamp values
  Number::Floating left = minimum.as<Number::Tagged>();
  Number::Floating right = maximum.as<Number::Tagged>();

  // finally handle our clamping as necessary
  return Number::Tagged(std::clamp(self.value(), left, right));
}

Sabre::Value::Any Sabre::Builtins::Field::is_nan(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args.self());
  return Value::Boolean(std::isnan(args.self<Number::Tagged>().value()));
}

Sabre::Value::Any Sabre::Builtins::Field::is_fin(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args.self());
  return Value::Boolean(std::isfinite(args.self<Number::Tagged>().value()));
}

Sabre::Value::Any Sabre::Builtins::Field::is_inf(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args.self());
  return Value::Boolean(std::isinf(args.self<Number::Tagged>().value()));
}

Sabre::Value::Any Sabre::Builtins::Field::stringify(Isolate *isolate, const Args &args, const $::String::View &method) {
  // prepare some baseline details about the arguments
  auto digits = args.at(0, Number::Tagged(0));
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, digits);
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args.self());

  // should be able to resolve our fraction and value
  Number::Floating value = args.self<Number::Tagged>();
  Number::Floating fraction = digits.as<Number::Tagged>();

  // fail if the fraction is outside our expected bounds
  auto exceeds = fraction < 0 || fraction > Number::Limits::FRACTION;
  if (exceeds) return isolate->panic(6000600, fmt::format("Number.{0}", method));

  // format our output as necessary now
  auto format = fmt::runtime(method.ends_with("precision") ? "{0:.{1}f}" : "{0:.{1}g}");
  return String::Any(isolate, fmt::format(format, value, static_cast<size_t>(fraction)));
}

Sabre::Value::Any Sabre::Builtins::Field::to_precision(Isolate *isolate, const Args &args) {
  if (!args.empty()) return stringify(isolate, args, "to_precision");
  return String::Literal::style(isolate, args.self()); // simple value
}

Sabre::Value::Any Sabre::Builtins::Field::to_scientific(Isolate *isolate, const Args &args) {
  // handle as normal if actually given a digit
  if (!args.empty()) return stringify(isolate, args, "to_scientific");

  // otherwise we need to do something different here
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args.self());

  // and convert as normal when given here
  return String::Any(isolate, fmt::format("{0:e}", args.self<Number::Tagged>().value()));
}

//  PRIVATE METHODS  //

Sabre::Member::View
Sabre::Builtins::Wrapper<Sabre::Number::Tagged>::m_attribute(const Number::Tagged &, const Value::Symbol &symbol) {
  return s_members.retrieve(symbol);
}
