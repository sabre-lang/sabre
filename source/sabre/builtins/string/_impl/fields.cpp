/// Sabre Includes
#include "sabre/locale/service.hpp"

/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  TYPEDEFS  //

#define SABRE_XX_FIELDS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Field, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/string/_defines/fields.def"

//  PROPERTIES  //

static auto s_members = Sabre::Builtins::Storage<Sabre::String::Any>({
#define SABRE_XX_FIELDS_DEFINE(N, ...) {#N, Sabre::Builtins::Field::N},
#include "sabre/builtins/string/_defines/fields.def"
});

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Field::symbol(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args.self());
  return args.self<String::Any>().symbol(); // get symbol
}

Sabre::Value::Any Sabre::Builtins::Field::size(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args.self());
  return Number::Tagged(args.self<String::Any>().size());
}

Sabre::Value::Any Sabre::Builtins::Field::bytes(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args.self());
  return Number::Tagged(args.self<String::Any>().bytes());
}

Sabre::Value::Any Sabre::Builtins::Field::empty(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args.self());
  return Number::Tagged(args.self<String::Any>().size() == 0);
}

Sabre::Value::Any Sabre::Builtins::Field::at(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args[0]);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args.self());

  // prepare the underlying self and numeric
  auto self = args.self<String::Any>();
  auto numeric = args.at<Number::Tagged>(0);

  // prepare the incoming index and size values
  auto index = static_cast<int64_t>(numeric);
  auto size = static_cast<int64_t>(self.size());

  // attempt updating the index if negative at all
  if (index < 0) index += size;

  // fail if the index is out of the available bounds
  SABRE_MM_ASSERT_INDEX(isolate, size, index);

  // attempt resolving the necessary rune value
  return String::Small(self.rune(index));
}

Sabre::Value::Any Sabre::Builtins::Field::slice(Isolate *isolate, const Args &args) {
  // prepare the underlying assertions
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args.self());

  // get the value to be sliced
  auto self = args.self<String::Any>();
  auto size = static_cast<int64_t>(self.size());

  // if not actually given any arguments, return self instead
  if (args.empty()) return self;

  // otherwise pull out our arguments needed
  auto slice = Iterable::Deduce::slice(isolate, args, size);
  if (!slice.has_value()) return Value::Failure();

  // stop early if the slice is potentially empty
  if (slice->empty()) return String::Any();

  // resolve the base indices from the runes
  auto start = self.offset(slice->start()), stop = self.offset(slice->stop());

  // and finally construct the resulting slice now
  return String::Any(isolate, self.view().substr(start, stop - start));
}

Sabre::Value::Any Sabre::Builtins::Field::compare(Isolate *isolate, const Args &args) {
  // prepare the underlying assertions
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args.self());

  // pull out our items in question now
  auto needle = args.at<String::Any>(0);
  auto haystack = args.self<String::Any>();

  // instead of using the base "compare" method we do locale comparison
  Locale::Service *locales = *isolate->service();
  auto result = locales->compare(haystack.view(), needle.view());

  // return the resulting comparison value now
  return Number::Tagged(result);
}

Sabre::Value::Any Sabre::Builtins::Field::contains(Isolate *isolate, const Args &args) {
  // prepare the underlying assertions
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args.self());

  // pull out our items in question now
  auto needle = args.at<String::Any>(0);
  auto haystack = args.self<String::Any>();

  // check if the haystack contains the needle
  return Value::Boolean(haystack.view().contains(needle.view()));
}

Sabre::Value::Any Sabre::Builtins::Field::starts_with(Isolate *isolate, const Args &args) {
  // prepare the underlying assertions
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args.self());

  // pull out our items in question now
  auto needle = args.at<String::Any>(0);
  auto haystack = args.self<String::Any>();

  // check if the haystack contains the needle
  return Value::Boolean(haystack.view().starts_with(needle.view()));
}

Sabre::Value::Any Sabre::Builtins::Field::ends_with(Isolate *isolate, const Args &args) {
  // prepare the underlying assertions
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args.self());

  // pull out our items in question now
  auto needle = args.at<String::Any>(0);
  auto haystack = args.self<String::Any>();

  // check if the haystack contains the needle
  return Value::Boolean(haystack.view().ends_with(needle.view()));
}

Sabre::Value::Any Sabre::Builtins::Field::to_upper(Isolate *isolate, const Args &args) {
  // ensure some incoming conditions now
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args.self());

  // pull out the incoming string to be modified
  auto self = args.self<String::Any>();

  // prepare a wide-string to conduct the conversion
  auto buffer = isolate->service<Locale::Service>()->uppercase(self.view());

  // return the resulting upper-case result now
  return String::Any(isolate, buffer);
}

Sabre::Value::Any Sabre::Builtins::Field::to_lower(Isolate *isolate, const Args &args) {
  // ensure some incoming conditions now
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args.self());

  // pull out the incoming string to be modified
  auto self = args.self<String::Any>();

  // prepare a wide-string to conduct the conversion
  auto buffer = isolate->service<Locale::Service>()->lowercase(self.view());

  // return the resulting upper-case result now
  return String::Any(isolate, buffer);
}

Sabre::Value::Any Sabre::Builtins::Field::trim_both(Isolate *isolate, const Args &args) {
  // ensure some incoming conditions now
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args.self());

  // pull out the incoming string to be modified
  auto self = args.self<String::Any>();

  // can safely trim as necessary
  return String::Any(isolate, $::Trim::both(self.view()));
}

Sabre::Value::Any Sabre::Builtins::Field::trim_leading(Isolate *isolate, const Args &args) {
  // ensure some incoming conditions now
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args.self());

  // pull out the incoming string to be modified
  auto self = args.self<String::Any>();

  // can safely trim as necessary
  return String::Any(isolate, $::Trim::leading(self.view()));
}

Sabre::Value::Any Sabre::Builtins::Field::trim_trailing(Isolate *isolate, const Args &args) {
  // ensure some incoming conditions now
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args.self());

  // pull out the incoming string to be modified
  auto self = args.self<String::Any>();

  // can safely trim as necessary
  return String::Any(isolate, $::Trim::trailing(self.view()));
}

Sabre::Value::Any Sabre::Builtins::Field::pad_leading(Isolate *isolate, const Args &args) {
  // ensure some incoming conditions now
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args[0]);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args.self());

  // pull out all the base values to be used
  auto self = args.self<String::Any>();
  auto maximum = static_cast<int64_t>(self.size());
  auto size = static_cast<int64_t>(args.at<Number::Tagged>(0));

  // resolve the padding string to be used
  String::Any padding = String::Small(" ");

  // we need to ensure we have the correct sizing
  if (args.size() > 1 && !args[1].is<Value::Void>()) {
    SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[1]);
    padding = args.at<String::Any>(1); // and resolve
  }

  // if the size does not exceed the current maximum, then copy instead
  if (size <= maximum) return String::Any(isolate, self.view());

  // prepare a buffer to be used now for padding
  auto buffer = $::String::Buffer();

  // whilst we have remaining characters, we can print
  for (auto remaining = size - maximum; remaining > 0;) {
    for (size_t uu = 0; uu < padding.size(); ++uu) {
      buffer += $::Encoding::UTF8::from(padding.rune(uu));
      if (--remaining <= 0) break; // break when necessary
    }
  }

  // and prepend the buffer now
  return String::Any(isolate, buffer + $::String::Buffer(self.view()));
}

Sabre::Value::Any Sabre::Builtins::Field::pad_trailing(Isolate *isolate, const Args &args) {
  // ensure some incoming conditions now
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, Number::Tagged, args[0]);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args.self());

  // pull out all the base values to be used
  auto self = args.self<String::Any>();
  auto maximum = static_cast<int64_t>(self.size());
  auto size = static_cast<int64_t>(args.at<Number::Tagged>(0));

  // resolve the padding string to be used
  String::Any padding = String::Small(" ");

  // we need to ensure we have the correct sizing
  if (args.size() > 1 && !args[1].is<Value::Void>()) {
    SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[1]);
    padding = args.at<String::Any>(1); // and resolve
  }

  // if the size does not exceed the current maximum, then copy instead
  if (size <= maximum) return String::Any(isolate, self.view());

  // prepare a buffer to be used now for padding
  auto buffer = $::String::Buffer();

  // whilst we have remaining characters, we can print
  for (auto remaining = size - maximum; remaining > 0;) {
    for (size_t uu = 0; uu < padding.size(); ++uu) {
      buffer += $::Encoding::UTF8::from(padding.rune(uu));
      if (--remaining <= 0) break; // break when necessary
    }
  }

  // and prepend the buffer now
  return String::Any(isolate, $::String::Buffer(self.view()) + buffer);
}

Sabre::Value::Any Sabre::Builtins::Field::fmt(Isolate *isolate, const Args &args) {
  // get the self instance to be used
  auto self = args.self();

  // ensure we validate the incoming details
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, self);

  // return immediately if there are no arguments given
  if (args.empty()) return self;

  // and attempt writing our details now
  return String::Literal::style(isolate, self.as<String::Any>(), args);
}

//  PRIVATE METHODS  //

Sabre::Member::View
Sabre::Builtins::Wrapper<Sabre::String::Any>::m_attribute(const String::Any &, const Value::Symbol &symbol) {
  return s_members.retrieve(symbol);
}
