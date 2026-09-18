/// Sabre Includes
#include "sabre/signal/service.hpp"
#include "sabre/runtime/container.hpp"

//  CONSTRUCTORS  //

Sabre::Signal::Service::Service() : Service($::Global::get<Runtime::Container>()) {}
Sabre::Signal::Service::Service(XI::Container *services) : m_async(*services) {
  m_async->signals()->attach([&](const Code &code) { m_dispatch(code); });
}

//  PRIVATE METHODS  //

void Sabre::Signal::Service::m_dispatch(const Code &code) {
  /// TODO: check if we can catch the incoming signal

  /// TODO: stop all the running threads from executing

  // if we reach here then we need to exit using the signal
  m_async->exit(code.errc());

  // for non-windows platforms we also print a newline
  if constexpr (!$_PLATFORM_WINDOWS) $::Debug::eprintln();
}
