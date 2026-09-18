/// Sabre Includes
#include "sabre/bundle/service.hpp"
#include "sabre/async/service.hpp"
#include "sabre/bundle/worker.hpp"
#include "sabre/runtime/container.hpp"

//  CONSTRUCTORS  //

Sabre::Bundle::Service::Service() : Service($::Global::get<Runtime::Container>()) {}
Sabre::Bundle::Service::Service(XI::Container *services) : m_services(services) {}

//  PUBLIC METHODS  //

int32_t Sabre::Bundle::Service::bundle(const Options &options) {
  Async::Service *async = *m_services; // prepare
  return async->launch<Worker>(m_services, options);
}
