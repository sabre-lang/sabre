/// Sabre Includes
#include "sabre/dylib/registry.hpp"
#include "sabre/runtime/isolate.hpp"

//  PUBLIC METHODS  //

Sabre::Dylib::Addon *Sabre::Dylib::Registry::install($::Unique::Pointer<Addon> &&addon) {
  // get the underlying name of the installer now
  auto *self = s_instance();
  auto name = addon->name();

  // ensure we lock emplacement
  $_UNUSED $_AUTO = $::Lock::guard(self->m_mutex);

  // ensure the emplacement is actually valid to be done
  $_ASSERT(!bound(name), "Library '{0}' already installed", name);
  $_TRACE("Dylib::Registry: Installing '{0}'", name); // report

  // and register the library for use
  return self->m_addons.emplace(name, std::move(addon)).first->second.get();
}

Sabre::Value::Any Sabre::Dylib::Registry::preload(Runtime::Isolate *isolate, const $::String::View &name) {
  const auto *self = s_instance(); // get instance
  $_UNUSED $_AUTO = $::Lock::guard(self->m_mutex);

  // declare an error for missing libraries if necessary here
  if (!bound(name)) return isolate->panic(8000100, fmt::format("{0}:{1}", Resource::Scheme::DYLIB, name));
  else return $_TRACE("Dylib::Registry: Preloading '{0}'", name), self->m_addons.at(name)->preload(isolate);
}

void Sabre::Dylib::Registry::unload(Runtime::Isolate *isolate, const $::String::View &name) {
  const auto *self = s_instance(); // get instance
  $_UNUSED $_AUTO = $::Lock::guard(self->m_mutex);
  if (!bound(name)) return; // ignore if not bound
  $_TRACE("Dylib::Registry: Unloading '{0}'", name);
  return self->m_addons.at(name)->unload(isolate);
}

//  PRIVATE METHODS  //

Sabre::Lifecycle::Service *Sabre::Dylib::Registry::m_lifecycle(Runtime::Isolate *isolate) {
  return isolate->service<Lifecycle::Service>();
}
