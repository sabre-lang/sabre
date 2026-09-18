/// Sabre Includes
#include "sabre/testing/entry.hpp"
#include "sabre/import/service.hpp"
#include "sabre/lifecycle/service.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/testing/service.hpp"

//  PRIVATE METHODS  //

$_NORETURN void Sabre::Testing::Entry::m_execute() {
  // prepare the baseline spinner suffix to be shown
  static auto s_suffix = $::Spinner::Suffix("Testing");

  // force our runtime to preload the lifecycle now
  $_UNUSED $_AUTO = m_services->get<Lifecycle::Service>()->scope(this);

  // get the underlying testing services to be used
  auto reporter = m_services->get<Diagnostic::Reporter>();

  // prepare the available thenables now
  auto thenables = $::Map::Base<$::URI::Buffer, Async::Thenable *>();

  // prepare the pattern to be used for finding tests (and a spinner)
  auto pattern = $::Glob::Pattern(options()->testing.pattern);

  // prepare the current spinner instance now
  if (options()->flags.verbose) m_spinner = service<Async::Service>()->spinner(s_suffix("Scanning Tests..."));

  // iterate over our testing entries to be evaluated
  for (const auto &entry : pattern.iterate()) {
    auto resource = $::URI::File(entry.path);
    auto *thenable = m_import(resource, reporter.get());
    thenables.emplace(resource, thenable);
  }

  // bail if there are no tests given
  if (thenables.empty()) m_empty(), m_thread->shutdown(EXIT_FAILURE);

  // if we had any type-errors, then cancel testing
  if (!m_report(reporter.get(), thenables.size())) m_thread->shutdown(EXIT_FAILURE);

  // prepare all the runtime-errors necessary
  auto unhandled = $::Map::Base<$::URI::View, Value::Any>();

  // wait for all the incoming thenables now as necessary
  for (const auto &[resource, thenable] : thenables) {
    if (thenable == nullptr) continue;
    auto result = thenable->await(thread());
    if (result.has_value()) continue;
    unhandled.emplace(resource, result.error());
  }

  // dismiss the current spinner instance if there is one
  if (m_spinner) m_spinner->dismiss(), m_spinner = nullptr;

  // bail if we have an unhandled errors before testing
  if (unhandled.size()) m_unhandled(std::move(unhandled)), m_thread->shutdown(EXIT_FAILURE);

  // and launch the testing session now
  m_thread->shutdown(m_services->get<Service>()->invoke(options()->testing));
}

bool Sabre::Testing::Entry::m_analyze(const $::URI::Buffer &resource, Diagnostic::Reporter *reporter) {
  if (options()->flags.typeless) return true; // required to ignore typings
  return !service<Import::Service>()->analyze(resource, reporter, false).errors;
}

void Sabre::Testing::Entry::m_empty() {
  if (m_spinner) m_spinner->dismiss(), m_spinner = nullptr;
  std::cerr << $::Dye::yellow("No tests found!") << '\n';
}

bool Sabre::Testing::Entry::m_report(Diagnostic::Reporter *reporter, size_t sources) {
  // get the total reporter details now as necessary
  auto errors = reporter->errors();
  auto hints = reporter->size() - errors;

  // show our reporter if it has a size
  if (errors || hints) {
    if (m_spinner) m_spinner->dismiss();
    std::cerr << *reporter << "\n\n";
  }

  // show the necessary final details for failures
  if (hints) $::Debug::eprintln("- Found {0} linting diagnostics", hints);
  if (errors) $::Debug::eprintln("- Found {0} error(s) across {1} files(s)", errors, sources);

  // post-print if necessary to do so
  if (hints && !errors) $::Debug::eprintln();

  // and declare whether or not this was a success/failure
  return !errors;
}

void Sabre::Testing::Entry::m_unhandled($::Map::Base<$::URI::View, Value::Any> &&errors) {
  std::cerr << $::Dye::red(errors.size()).bold() << ' '; // show total errors
  std::cerr << $::Dye::red("Unhandled Error{0}", errors.size() == 1 ? "" : "s").bold();
  std::cerr << $::Dye::dim(" between tests occured. See below:\n"); // and reasons

  // and show all the unhandled errors that occured
  for (const auto &[resource, reason] : errors) m_unhandled(resource, reason);
}

void Sabre::Testing::Entry::m_unhandled(const $::URI::View &resource, const Value::Any &reason) {
  // convert the reason to an exception now
  auto exception = (panic(reason), m_exception.as<Object::Exception>());

  // get the relative resource value to be shown
  auto relative = resource.relative().string();
  auto fence = relative.size() + 3; // padding

  // show that this file had an unhandled exception
  std::cerr << $::Dye::dim("\n# {0}:", relative) << '\n';
  std::cerr << $::Dye::dim($::String::Buffer(fence, '-')) << '\n';
  std::cerr << $::Dye::red("Unhandled "), exception.format(std::cerr) << "\n";

  // post-reset the underlying exception now
  m_exception = Value::Void();
}

Sabre::Async::Thenable *
Sabre::Testing::Entry::m_import(const $::URI::Buffer &resource, Diagnostic::Reporter *reporter) {
  if (!m_analyze(resource, reporter)) return nullptr;
  return service<Import::Service>()->import(this, resource);
}
