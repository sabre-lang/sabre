/// Sabre Includes
#include "sabre/lifecycle/scope.hpp"
#include "sabre/lifecycle/service.hpp"
#include "sabre/runtime/container.hpp"

//  CONSTRUCTORS  //

Sabre::Lifecycle::Scope::Scope(Runtime::Isolate *isolate) : Scope($::Global::get<Runtime::Container>(), isolate) {}
Sabre::Lifecycle::Scope::Scope(XI::Container *services, Runtime::Isolate *isolate) :
    m_lifecycle(*services), m_isolate(isolate) {
  m_lifecycle->preload(isolate); // bind now
}

Sabre::Lifecycle::Scope::~Scope() { m_lifecycle->unload(m_isolate); }
