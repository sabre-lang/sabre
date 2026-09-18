/// Sabre Includes
#include <sabre/toolchain/format.hpp>

/// Shell Includes
#include "shell/format/action.hpp"

//  PRIVATE METHODS  //

Shell::Format::Result Shell::Format::Action::m_worker(const $::URI::Buffer &resource) const {
  auto buffer = Sabre::Document::Buffer(resource);
  return Sabre::Toolchain::format(buffer, m_options);
}

void Shell::Format::Action::m_overwrite(const $::URI::View &resource, const Result &result) const {
  if (result.has_value()) $::FS::Overwrite(resource.body(), *result);
  else m_fail("Failed to parse '{0}'", resource.relative());
}

void Shell::Format::Action::m_output(const $::URI::View &resource, const Result &result, bool edits) const {
  // handle based on the edits details now
  if (!edits) return result ? $::Debug::print(*result) : m_fail("Failed to parse '{0}'", resource.relative());

  // if we failed to parse, the declare a "null" result for JSON edits
  if (!result.has_value()) return $::Debug::println($::JSON::Encode($::Serde::Value()).value());

  /// TODO: otherwise convert the result into suitable edits to be used
  $_ABORT("Unimplemented --json formatter edits");
}
