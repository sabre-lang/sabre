/// Sabre Includes
#include "sabre/diagnostic/reporter.hpp"
#include "sabre/crate/registry.hpp"
#include "sabre/runtime/options.hpp"

//  CONSTRUCTORS  //

Sabre::Diagnostic::Reporter::Reporter(const $::URI::Buffer &resource) : Reporter(Options(), resource) {}

Sabre::Diagnostic::Reporter::Reporter(const Options &options, const $::URI::Buffer &resource) :
    m_resource(resource), m_options($::Unique::New<Options>(options)) {
  auto crate = Crate::Registry::scan(resource); // get now
  if (crate) m_options->overrides = crate->lint().overrides;
}

Sabre::Diagnostic::Reporter::Reporter(XI::Container *services, const $::URI::Buffer &resource) :
    Reporter(services->get<Runtime::Options>()->diagnostics, resource) {}

//  PRIVATE METHODS  //

Sabre::Diagnostic::Severity Sabre::Diagnostic::Reporter::m_severity(Code code) const {
  return m_options->overrides.contains(code) ? m_options->overrides.at(code) : Inspect::severity(code);
}

bool Sabre::Diagnostic::Reporter::m_ignore(const View &diagnostic) const {
  if (m_diagnostics.size() > limit()) return true; // should ignore since limit is reached
  if (diagnostic.m_stack.empty()) return true;     // ignore if the underlying stack is empty
  return diagnostic.m_severity == Severity::NONE || diagnostic.m_severity > severity();
}

bool Sabre::Diagnostic::Reporter::m_exists(const View &diagnostic) const {
  auto predicate = [diagnostic](const View &other) { return diagnostic == other; };
  return std::ranges::any_of(m_diagnostics, predicate); // check if the other exists
}

Sabre::Diagnostic::View Sabre::Diagnostic::Reporter::m_create(Code code, Resource::Stack stack) const {
  auto view = View(code, m_severity(code), stack); // build
  return $::Ranges::Prepend(view.m_stack, m_scoping), view;
}

void Sabre::Diagnostic::Reporter::m_print(std::ostream &os, const Reporter &self) {
  for (const auto &[ii, diagnostic] : $::Ranges::Each(self.m_diagnostics)) {
    os << diagnostic; // show the diagnostic and ending
    if (ii < self.m_diagnostics.size() - 1) os << "\n\n";
  }
}