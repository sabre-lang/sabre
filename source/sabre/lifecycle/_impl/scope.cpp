/// Sabre Includes
#include "sabre/lifecycle/scope.hpp"
#include "sabre/lifecycle/service.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/runtime/isolate.hpp"

//  CONSTRUCTORS  //

Sabre::Lifecycle::Scope::Scope(Runtime::Isolate *isolate) : m_isolate(isolate) {
  isolate->service<Service>()->preload(isolate);
}

Sabre::Lifecycle::Scope::~Scope() { m_isolate->service<Service>()->unload(m_isolate); }
