/// Sabre Includes
#include "sabre/lifecycle/service.hpp"
#include "sabre/async/service.hpp"
#include "sabre/dylib/registry.hpp"
#include "sabre/garbage/service.hpp"
#include "sabre/globals/service.hpp"
#include "sabre/import/service.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/runtime/executor.hpp"

/// Builtin Includes
#include "sabre/builtins/_inline/builtins.ipp"

//  CONSTRUCTORS  //

Sabre::Lifecycle::Service::Service() : Service($::Global::get<Runtime::Container>()) {}
Sabre::Lifecycle::Service::Service(XI::Container *services) : m_services(services) {}

//  PUBLIC METHODS  //

void Sabre::Lifecycle::Service::preload(Runtime::Isolate *isolate) {
  // get the available global service now
  Async::Service *async = *m_services;
  Import::Service *modules = *m_services;
  Globals::Service *globals = *m_services;

  // we need to pre-instantiate the "Object" global (to ensure class ordering)
  globals->get(isolate, Builtins::Inspect<Object::Instance>::name());

  // we need our roots service for help with garbage collection
  auto *roots = globals->roots();

  // register all the "dylib" modules as well
  roots->bind([](Globals::Each &yield) { Dylib::Registry::each(yield); });

  // all runtime isolates must have access to their roots
  roots->bind([async](Globals::Each &yield) {
    for (const auto &thread : async->scheduler()->threads()) {
      if (!thread->is<Runtime::Executor>()) continue; // cast now as needed
      reinterpret_cast<Runtime::Executor *>(thread->task())->roots(yield);
    }
  });

  // register all the modules with access to their roots
  roots->bind([modules](Globals::Each &yield) {
    for (const auto &module : modules->storage()->each()) {
      auto *exports = module->metadata<Module::Phase::EXPORTED>();
      for (auto &scope : exports->scopes()) yield(scope);
      auto *deferred = exports->thenable(); // resolve now
      if (!deferred->pending()) yield(deferred->immediate());
    }
  });
}

void Sabre::Lifecycle::Service::unload(Runtime::Isolate *) {
  // ensure each individual disposable is executed
  for (const auto &disposable : m_disposables | std::views::values) disposable();

  // remove the disposables now since all finalized
  m_disposables.clear();

  // prepare a mapping for the removed headers
  auto removed = $::Map::Set<Pointer::Underlying>();

  // also ensure we deallocate all heap-allocated values before exiting as well
  m_services->get<Garbage::Service>()->cleanup();
}
