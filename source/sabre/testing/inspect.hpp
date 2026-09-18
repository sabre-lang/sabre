#ifndef _SABRE_TESTING_INSPECT_HPP
#define _SABRE_TESTING_INSPECT_HPP

/// Sabre Modules
#include "sabre/forward/testing.hpp"
#include "sabre/object/exception.hpp"
#include "sabre/resource/trace.hpp"

namespace Sabre::Testing::Inspect {

/**
 * @brief Resolves a location from a resource trace.
 * @param trace                 Trace to resolve.
 */
inline constexpr XT::Trivia::Location location(const Resource::Trace &trace) {
  auto *position = trace.position(); // resolve baseline
  if (position == nullptr) return {trace.resource(), 0, 0};
  return {trace.resource(), position->line, position->column};
}

/**
 * @brief Derives a location from an exception.
 * @param exception             Exception to derive.
 * @param filter                Filters evaluation schemes.
 */
inline constexpr XT::Trivia::Location location(const Object::Exception &exception, bool filter = true) {
  // prepare a suitably filter predicate
  auto predicate = [filter](const Resource::Trace &trace) {
    if (trace.anonymous()) return false; // always fail anonymous items
    return !filter || trace.resource().scheme() != $::URI::Scheme::EVAL;
  };

  // iterate through the exception to find a suitable location
  for (const auto &trace : $::Ranges::Filter(exception.trace(), predicate)) return location(trace);

  // otherwise we need to an empty trace
  return {};
}

} // namespace Sabre::Testing::Inspect

#endif
