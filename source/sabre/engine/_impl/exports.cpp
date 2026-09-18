/// Sabre Includes
#include "sabre/engine/exports.hpp"
#include "sabre/object/instance.hpp"
#include "sabre/runtime/isolate.hpp"

//  PRIVATE METHODS  //

Sabre::Value::Any Sabre::Engine::Exports::m_open(Runtime::Isolate *isolate) {
  return m_scopes.emplace_back(isolate->create<Object::Instance>());
}

Sabre::Value::Any Sabre::Engine::Exports::m_close() {
  auto value = m_scopes.back();
  return m_scopes.pop_back(), value;
}
