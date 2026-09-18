/// Sabre Includes
#include "sabre/crate/registry.hpp"
#include "sabre/crate/constants.hpp"
#include "sabre/document/buffer.hpp"

//  PUBLIC METHODS  //

$::Unique::Pointer<Sabre::Crate::Manifest> Sabre::Crate::Registry::global() { return $::Unique::New<Manifest>(); }

$::Unique::Pointer<Sabre::Crate::Manifest> Sabre::Crate::Registry::scan(const $::URI::View &hint) {
  return hint.scheme() == $::URI::Scheme::FILE ? scan(hint.body()) : global();
}

$::Unique::Pointer<Sabre::Crate::Manifest> Sabre::Crate::Registry::scan(const $::FS::Path &hint) {
  // prepare the parent path to be used
  $::FS::Path parent = hint, root = $::System::root();

  // attempt scanning upwards for a suitable crate-file
  auto query = hint / Crate::Constants::filename();

  // iterate over the available paths now
  while (!$::Path::exists(query) && parent != root) {
    parent = $::Path::dirname(parent); // jump
    query = parent / Crate::Constants::filename();
  }

  // and attempt resolving whether the query exists or not
  return $::Path::exists(query) ? view(query) : global();
}

$::Unique::Pointer<Sabre::Crate::Manifest> Sabre::Crate::Registry::view(const $::FS::Path &file_path) {
  return view($::URI::Buffer(file_path.string()));
}

$::Unique::Pointer<Sabre::Crate::Manifest> Sabre::Crate::Registry::view(const $::URI::View &resource) {
  // prepare the buffer to be read now
  auto buffer = Document::Buffer(resource);
  auto json = $::JSON::Decode(buffer.view());

  // fail early if there are any JSONC errors found
  if (!json || json->is<$::Serde::Null>()) return nullptr;

  // prepare the path to be bound to the instance
  auto path = $::Path::dirname(resource.body()).string();

  // and attempt decoding into our instance now
  return $::Serde::Decode<Manifest>(*json, path);
}
