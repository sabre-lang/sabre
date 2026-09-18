/// Sabre Includes
#include "sabre/crate/manifest.hpp"

//  PRIVATE METHODS  //

$::Unique::Pointer<Sabre::Crate::Manifest> Sabre::Crate::Manifest::m_decode(const $::Serde::Value &value) {
  // although we default the path this is normally recommended
  return m_decode(value, $::System::root());
}

$::Unique::Pointer<Sabre::Crate::Manifest>
Sabre::Crate::Manifest::m_decode(const $::Serde::Value &value, const $::String::Buffer &path) {
  // prepare the baseline instance to be used
  auto self = $::Unique::New<Manifest>(path);

  // fast-path to earlier instances if necessary
  if (!value.is<$::Serde::Object>()) return self;

  // destructure the basic manifest options
  if (auto *name = value.at<$::Serde::Text>("name")) self->m_name = *name;
  if (auto *main = value.at<$::Serde::Text>("main")) self->m_main = *main;
  if (auto *version = value.at<$::Serde::Text>("version")) self->m_version = *version;

#define X(N, T, ...)                               \
  if (auto *N = value.at<T>(#N)) self->m_##N = *N;
  SABRE_XX_CRATE_OPTIONALS(X) // set the optional values as necessary now
#undef X

#define X(N, T, ...)                                                                   \
  if (auto *N = value.at<$::Serde::Object>(#N)) self->m_##N = $::Serde::Decode<T>(*N);
  SABRE_XX_CRATE_DICTIONARIES(X) // attempt reading all the dictionary values now
#undef X

  // destructure the more complex items now
  self->m_lint = $::Serde::Decode<Relint::Options>(value.at("lint"));
  self->m_format = $::Serde::Decode<Format::Options>(value.at("format"));

  // and return the resulting output now
  return self;
}
