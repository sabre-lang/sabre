/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  TYPEDEFS  //

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/exception/_defines/statics.def"

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Builtins::Static::from(Isolate *isolate, const Args &args) {
  // stop early if given no arguments at all
  if (args.empty()) return isolate->create<Object::Exception>();

  // ensure some conditions about the incoming details
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);

  // pull out some details from the arguments
  auto message = args.at<String::Any>(0);

  // attempt formatting the exception now
  auto result = String::Literal::style(isolate, message, args.slice(1));

  // check if the incoming message is okay, if not then stop early
  if (!result.pointer().okay() || !result.is<String::Any>()) return result;

  // and resolve the exception to be constructed
  return isolate->create<Object::Exception>(result.as<String::Any>());
}

Sabre::Value::Any Sabre::Builtins::Static::named(Isolate *isolate, const Args &args) {
  // ensure some conditions about the incoming details
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);
  if (args.size() > 1) SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[1]);

  // pull out some details from the arguments
  auto name = args.at<String::Any>(0);

  // attempt getting a suitable message now
  if (args.size() == 1) return isolate->create<Object::Exception>(name, String::Any());

  // ensure we validate the incoming message value now
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[1]);

  // update the incoming message now
  auto message = args.at<String::Any>(1);

  // attempt formatting the exception now
  auto result = String::Literal::style(isolate, message, args.slice(2));

  // check if the incoming message is okay, if not then stop early
  if (!result.pointer().okay() || !result.is<String::Any>()) return result;

  // and resolve the exception to be constructed
  return isolate->create<Object::Exception>(name, result.as<String::Any>());
}

//  PRIVATE METHODS  //

Sabre::Value::Any
Sabre::Builtins::Wrapper<Sabre::Object::Exception>::m_globals(Isolate *isolate, const Object::Class &self) {
#define SABRE_XX_STATICS_DEFINE(N, ...)                                                \
  self.statics().emplace(#N, Member::Factory::native(isolate, Static::N, name(), #N));
#include "sabre/builtins/exception/_defines/statics.def"

  // and return the resulting instance
  return self;
}
