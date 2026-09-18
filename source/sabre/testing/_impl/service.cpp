/// Sabre Includes
#include "sabre/testing/service.hpp"
#include "sabre/async/service.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/testing/entry.hpp"

//  CONSTRUCTORS  //

Sabre::Testing::Service::Service() : Service($::Global::get<Runtime::Container>()) {}
Sabre::Testing::Service::Service(XI::Container *services) : m_services(services), m_storage(*m_services) {}

//  PRIVATE METHODS  //

int32_t Sabre::Testing::Service::m_spawn() { return m_services->get<Async::Service>()->launch<Entry>(m_services); }

int32_t Sabre::Testing::Service::m_invoke(const Options &options) {
  // declare an error if a session is in progress
  $_ASSERT(m_session == nullptr, "Testing session already in progress");

  // prepare the testing options to be bound (we allow some overrides)
  XT::Session::Options passthrough = {
      .bench = options.bench,
      .title = SABRE_MM_IDENTIFIER "/testing",
      .reporter = {
          .output = std::cerr,
          .spinner = [&](const $::Spinner::Options &options) {
            return m_services->get<Async::Service>()->spinner(options);
          },
      },
  };

  // prepare the session instance now
  auto runner = Runner(&passthrough);

  // prepare a deferred removal of the session instance
  $_UNUSED $_AUTO = $::Lambda::Defer([&] { m_session = nullptr; });

  // and launch the testing instance
  return XT::Bootstrap::launch(m_session = &runner, m_storage->sections());
}
