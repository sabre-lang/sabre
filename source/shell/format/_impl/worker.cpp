/// Sabre Includes
#include <sabre/crate/constants.hpp>
#include <sabre/crate/registry.hpp>
#include <sabre/toolchain/format.hpp>

/// Shell Includes
#include "shell/format/action.hpp"

//  PRIVATE METHODS  //

Shell::Format::Result Shell::Format::Action::m_worker(const $::URI::Buffer &resource) const {
  // we need to resolve the internal options to be used
  auto options = m_options.has_value() ? *m_options : Sabre::Crate::Registry::scan(resource)->format();

  // and finally delegate the requried formatting details now
  auto buffer = Sabre::Document::Buffer(resource);
  return Sabre::Toolchain::format(buffer, options);
}

std::optional<Shell::Format::Options> Shell::Format::Action::m_resolve(const $::String::View &config) const {
  return m_resolve($::URI::File(config));
}

std::optional<Shell::Format::Options> Shell::Format::Action::m_resolve(const $::URI::Buffer &resource) const {
  // prepare a warning handler when missing
  auto s_warn = [&](const $::URI::View &view) {
    return m_warn("Configuration '{0}' does not exist", view.relative()), std::nullopt;
  };

  // handle incoming resolutions for reference values
  switch (XH::FNV::U32(resource.scheme())) {
  case XH::FNV::U32($::URI::Scheme::FILE): break;
  default: m_fail("Invalid configuration value '{0}'", resource.relative()); break;
  case XH::FNV::U32($::URI::Scheme::HREF): {
    auto absolute = $::Path::absolute(resource.body());
    if (absolute.empty()) return s_warn(resource);
    return m_resolve($::URI::Buffer(absolute.string()));
  }
  }

  // get the absolute path to be used now
  auto crate = $::FS::Path(resource.body());

  // check if we have a suitable crate-file now
  if ($::Path::is_directory(crate)) crate = $::Path::join(crate, Sabre::Crate::Constants::filename());

  // validate the crate-file actually exists now
  auto manifest = Sabre::Crate::Registry::view(crate);
  if (manifest == nullptr) return s_warn(resource);

  // allow updating the desired formatting options now
  return manifest->format();
}

void Shell::Format::Action::m_overwrite(const $::URI::View &resource, const Result &result) const {
  if (result.has_value()) $::FS::Overwrite(resource.body(), *result);
  else m_fail("Failed to parse '{0}'", resource.relative());
}

void Shell::Format::Action::m_output(const $::URI::View &resource, const Result &result) const {
  return result ? $::Debug::print(*result) : m_fail("Failed to parse '{0}'", resource.relative());
}
