/// Sabre Includes
#include "sabre/crate/service.hpp"
#include "sabre/crate/registry.hpp"
#include "sabre/dylib/registry.hpp"

//  PUBLIC METHODS  //

bool Sabre::Crate::Service::validate(const $::FS::Path &resource) {
  return $::Path::filename(resource) == Constants::filename();
}

const Sabre::Crate::Manifest *Sabre::Crate::Service::resolve(const $::FS::Path &file_path) {
  return resolve($::URI::Buffer(file_path.string()));
}

const Sabre::Crate::Manifest *Sabre::Crate::Service::resolve(const $::URI::View &resource) {
  // lock the incoming crates mutex now
  $_AUTO $_UNUSED = $::Lock::guard(m_mutex);

  // resolve the key to be used currently
  auto key = $::String::Buffer(resource.view());

  // check if the resource is currently available
  if (m_storage.contains(key)) return m_storage.at(key).get();

  // and emplace the crate-file now
  return m_storage.emplace(key, Registry::view(resource)).first->second.get();
}

Sabre::Value::Any Sabre::Crate::Service::dylib(Runtime::Isolate *isolate, const $::String::View &name) const noexcept {
  return Dylib::Registry::preload(isolate, name);
}
