/// Sabre Includes
#include "sabre/import/service.hpp"
#include "sabre/async/service.hpp"
#include "sabre/crate/service.hpp"
#include "sabre/diagnostic/reporter.hpp"
#include "sabre/document/service.hpp"
#include "sabre/forward/crate.hpp"
#include "sabre/resource/frame.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/runtime/isolate.hpp"

/// Module Includes
#include "sabre/module/archive.hpp"
#include "sabre/module/dynamic.hpp"
#include "sabre/module/script.hpp"

/// Forward Declarations
$_FWD(Sabre::Resource, template <class... As> Result Failure(Diagnostic::Code, As &&...))
$_FWD(Sabre::Resource, template <$::String::Literal> Result Resolve(const $::String::View &, const $::FS::Path &))

//  MACROS  //

#define MM_RESOLVE_SCHEME(S, R, H, ...)                                         \
  template <>                                                                   \
  Sabre::Resource::Result Sabre::Resource::Resolve<Sabre::Resource::Scheme::S>( \
      const $::String::View &R, const $::FS::Path &H                            \
  )

//  CONSTRUCTORS  //

Sabre::Import::Service::Service() : Service($::Global::get<Runtime::Container>()) {}
Sabre::Import::Service::Service(XI::Container *services) :
    m_services(services), m_crates(*m_services), m_documents(*m_services), m_storage(*m_services) {
  // bind some baseline loaders to be used for imports now
  subscribe<Loader::Define<Resource::Scheme::FILE, Module::Script>>();
  subscribe<Loader::Define<Resource::Scheme::DYLIB, Module::Dynamic>>();
}

//  PUBLIC METHODS  //

MM_RESOLVE_SCHEME(FILE, absolute, ) {
  // prepare a suitable relative path
  auto relative = $::Path::relative(absolute);

  // ensure the file-path actually exists now
  if (!$::Path::exists(absolute)) return Failure(8000100, relative.string());

  // check if we do not have a directory value now
  if (!$::Path::is_directory(absolute)) return $::URI::Buffer(absolute);

  // otherwise we attempt resolving as a crate-file now
  auto crate = $::Path::join(absolute, Crate::Constants::filename());

  // check if the incoming path actually exists
  auto exists = $::Path::exists(crate);
  if (exists) return $::URI::Buffer(crate.string());
  return Failure(8000100, relative.string());
}

MM_RESOLVE_SCHEME(HREF, relative, hint) {
  auto absolute = $::Path::absolute(relative, hint);
  if (absolute.empty()) return Failure(8000100, relative);
  return Resolve<Scheme::FILE>(absolute.string(), hint);
}

MM_RESOLVE_SCHEME(ARCH, relative, ) { return $::URI::Buffer(Scheme::ARCH, relative); }
MM_RESOLVE_SCHEME(DYLIB, name, ) { return $::URI::Buffer(Scheme::DYLIB, name); }

MM_RESOLVE_SCHEME(SABRE, name, ) {
  auto crate = Crate::Constants::internal(name); // resolve item
  if ($::Path::exists(crate)) return $::URI::Buffer(crate.string());
  return Failure(8000100, Scheme::SABRE.buffer() + ":" + $::String::Buffer(name));
}

MM_RESOLVE_SCHEME(CRATE, name, ) {
  auto crate = Crate::Constants::external(name); // resolve item
  if ($::Path::exists(crate)) return $::URI::Buffer(crate.string());
  return Failure(8000100, Scheme::CRATE.buffer() + ":" + $::String::Buffer(name));
}

template <class... As> Sabre::Resource::Result Sabre::Resource::Failure(Diagnostic::Code code, As &&...args) {
  return std::unexpected(Diagnostic::Inspect::format(code, std::forward<As>(args)...));
}

Sabre::Resource::Result Sabre::Import::Service::resolve(const $::String::View &script, const $::FS::Path &hint) const {
  auto result = m_resolve(script, hint);
  return result ? resolve(*result) : result;
}

Sabre::Resource::Result Sabre::Import::Service::resolve(const $::URI::Buffer &resource) const {
  // stop early when we do not have a file resource (fully done)
  if (resource.scheme() != Resource::Scheme::FILE) return resource;

  // get the associated file-path to be checked against a crate for
  auto absolute = $::Path::canonical(resource.body());

  // if we have a non-crate manifest, then return immediately
  if (!m_crates->validate(absolute)) return resource;

  auto *crate = m_crates->resolve(absolute); // attempt resolving the crate
  if (auto main = crate->main()) return resolve(main->body(), crate->path());
  return Resource::Failure(8000100, resource.relative().string());
}

Sabre::Module::Abstract *Sabre::Import::Service::fetch(const $::URI::Buffer &resource) {
  // pre-fail if there is already an unresolved scheme found
  $_ASSERT(resource.scheme() != $::URI::Scheme::HREF, "Cannot fetch unresolved 'href' resources");

  // check if the resource exists currently
  if (auto *module = m_storage->lookup(resource)) return m_prepare(module);

  // attempt getting the associated loader now
  auto iter = m_loaders.find(resource.scheme());
  if (iter == m_loaders.end()) return nullptr;

  // resolve the module instance before emplacing
  auto module = iter->second->fetch(m_services, resource);
  return m_prepare(m_storage->emplace(std::move(module)));
}

void Sabre::Import::Service::preload(std::vector<$::Unique::Pointer<Image::Arena>> &&arenas) {
  for (auto &&arena : arenas) m_storage->emplace($::Unique::New<Module::Archive>(std::move(arena)));
}

Sabre::Async::Thenable *Sabre::Import::Service::import(
    Runtime::Isolate *isolate, const $::URI::Buffer &resource, const Resource::Trace &trace
) {
  // attempt getting the module to be imported
  auto *module = fetch(resource);

  // if the module is invalid, then declare as such
  if (module == nullptr) return isolate->panic(8000102, resource.relative()), nullptr;

  // ensure we have a module that is in the correct state before continuing
  switch (module->phase()) {
  // ensure we parse and analyze the incoming module now
  case Module::Phase::CLEANED: module->parse(m_services); $_FALLTHROUGH;
  case Module::Phase::PARSED: module->analyze(m_services); $_FALLTHROUGH;

  // break when we have typed/compiled outputs
  case Module::Phase::TYPED: $_FALLTHROUGH;
  case Module::Phase::COMPILED: break;

  // if the module has already been exported, then we wait for the result
  default: return module->metadata<Module::Phase::EXPORTED>()->thenable();
  }

  // check how many errors have occurred now
  auto errors = module->reporter()->errors();

  // stop when the module has any incoming errors
  if (errors) return isolate->panic(8000001, resource.relative(), errors), nullptr;

  // ensure the module has been suitably compiled as well
  if (!module->ready()) module->compile(m_services);

  // prepare the resource frame to be used
  $_UNUSED $_AUTO = Resource::Frame(isolate, trace);

  // and attempt interpretting the result now
  return module->interpret(isolate);
}

Sabre::Import::Statistics Sabre::Import::Service::analyze(const $::URI::Buffer &resource, bool report) {
  return analyze(std::vector({resource}), report);
}

Sabre::Import::Statistics
Sabre::Import::Service::analyze(const $::URI::Buffer &resource, Diagnostic::Reporter *reporter, bool display) {
  return analyze(std::vector({resource}), reporter, display);
}

Sabre::Import::Statistics Sabre::Import::Service::analyze(const Dependencies &dependencies, bool report) {
  auto reporter = report ? m_services->get<Diagnostic::Reporter>() : nullptr;
  return analyze(dependencies, reporter.get(), report); // handle analyzing now
}

Sabre::Import::Statistics
Sabre::Import::Service::analyze(const Dependencies &dependencies, Diagnostic::Reporter *collector, bool display) {
  // prepare the expected severity value
  size_t errors = 0, hints = 0, sources = 0;

  // prepare the spinner to be used
  $::Spinner::Pointer spinner = nullptr;

  // get the required services to be used
  Async::Service *async = *m_services;
  Runtime::Options *options = *m_services;

  // check if using verbose output at all
  auto verbose = collector && options->flags.verbose && display;

  // prepare the suffix constructor to be used now
  static auto s_suffix = $::Spinner::Suffix("Analyzing");

  // prepare a spinner instance if necessary
  if (verbose) spinner = async->spinner(s_suffix("Loading Includes..."));

  // get an initial starting time-point
  auto start = $::Clock::Performance();

  // prepare the underlying graph-view
  auto view = graph(dependencies);
  auto leaves = view->leaves();

  // analyze our leaves in reverse breadth-first search order
  for (auto *module : std::views::reverse(leaves)) {
    // show the current module being checked here
    if (spinner) spinner->suffix(s_suffix("'" + module->resource().relative().string() + "'"));

    // analyze the module now
    module->analyze(m_services);

    auto *reporter = module->reporter(); // get the diagnostics
    auto count = reporter->errors();     // count total errors
    hints += reporter->size() - count;   // keep track of hints

    // update the current statistics to be shown
    if (count) errors += count, ++sources;
    if (collector) collector->collect(*reporter);
  }

  // show the baseline spinner as well if necessary
  if (spinner) {
    auto elapsed = $::Clock::Performance() - start; // get time
    static constexpr auto message = "Analyzed {0} module(s) in {1}";
    spinner->dismiss(s_suffix(fmt::format(message, leaves.size(), elapsed)));
  }

  // report our errors if they have been found at all
  if (display && collector && collector->size()) $::Debug::eprintln("{0}{1}\n", spinner ? "\n" : "", *collector);

  // show the verbose details when necessary too
  if (display && collector) {
    if (hints) $::Debug::eprintln("- Found {0} linting diagnostic(s)", hints);
    if (errors) $::Debug::eprintln("- Found {0} error(s) across {1} file(s)", errors, sources);
  }

  // return the final errors count
  return {.errors = errors, .hints = hints};
}

//  PRIVATE METHODS  //

Sabre::Module::Abstract *Sabre::Import::Service::m_prepare(Module::Abstract *module) const {
  if (module == nullptr) return module; // passthrough
  if (module->phase() >= Module::Phase::PARSED) return module;
  return module->parse(m_services), module; // forcibly parse
}

Sabre::Resource::Result
Sabre::Import::Service::m_resolve(const $::String::View &script, const $::FS::Path &hint) const {
  // start by parsing the baseline resource
  auto resource = $::URI::Parse(script);

  // get the baseline body we are dealing with
  auto body = $::Trim::both(resource.body());

  // and ensure that it is not empty
  if (body.empty()) return Resource::Failure(8000101);

  // get the current scheme to be used
  auto scheme = resource.scheme();

// then resolve against the baseline schemes
#define X(N, S, ...)                                                               \
  case XH::FNV::U32(S): return Resource::Resolve<Resource::Scheme::N>(body, hint);
  switch (XH::FNV::U32(scheme)) { SABRE_XX_RESOURCE_SCHEMES(X) default : break; }
#undef X

  // if we reach here, then we need to check against user-defined loaders
  auto iter = m_loaders.find(scheme);
  auto exists = iter != m_loaders.end();

  // and then handle the dynamic resolution now
  if (exists) return iter->second->resolve(body, hint);
  else return Resource::Failure(8000103, scheme);
}