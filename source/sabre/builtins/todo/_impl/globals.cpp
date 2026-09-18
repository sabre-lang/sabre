/// Sabre Includes
#include "sabre/function/native.hpp"
#include "sabre/runtime/isolate.hpp"

/// Builtin Includes
#include "sabre/builtins/_inline/builtins.ipp"

//  PRIVATE METHODS  //

Sabre::Value::Any Sabre::Builtins::Wrapper<Sabre::Builtins::Custom::Todo>::m_globals(Isolate *isolate) {
  return isolate->create<Function::Native>(m_panic, name());
}

Sabre::Value::Any Sabre::Builtins::Wrapper<Sabre::Builtins::Custom::Todo>::m_panic(Isolate *isolate, const Args &args) {
  // get the baseline argument to be handled now
  auto reason = args.at(0, Value::Void());

  // handle the incoming argument based on its typing now
  if (reason.is<Value::Void>()) return isolate->todo();
  if (reason.is<String::Any>()) return isolate->todo(reason.as<String::Any>());

  // otherwise throw a typing error now since invalid
  return isolate->panic(3000301, reason.brand(), "String?");
}
