/// Sabre Includes
#include "sabre/bundle/worker.hpp"
#include "sabre/bundle/fuse.hpp"
#include "sabre/bundle/service.hpp"
#include "sabre/document/buffer.hpp"
#include "sabre/import/service.hpp"
#include "sabre/lifecycle/service.hpp"

//  CONSTRUCTORS  //

Sabre::Bundle::Worker::Worker(const Options &options) : Entry(), m_options(options) {}
Sabre::Bundle::Worker::Worker(XI::Container *services, const Options &options) :
    Entry(services), m_options(options), m_archive(*services) {}

//  PRIVATE METHODS  //

$_NORETURN void Sabre::Bundle::Worker::m_execute() {
  // force our runtime to preload the lifecycle now
  $_UNUSED $_AUTO = service<Lifecycle::Service>()->scope(this);

  // prepare a suitable spinner suffix to be used
  static auto s_suffix = $::Spinner::Suffix("Bundling");

  // prepare the necessary services to be used
  auto *async = service<Async::Service>();
  auto *runtime = service<Runtime::Options>();
  auto *modules = service<Import::Service>();

  // start by resolving the underlying script
  auto script = m_resolve(runtime->script.entry);
  $::Unique::Pointer<Diagnostic::Reporter> reporter = *m_services;

  // attempt running analysis now
  auto stats = runtime->flags.typeless ? Import::Statistics() : modules->analyze(script, reporter.get(), true);

  // attempt checking the types available now as necessary
  if (stats.errors) m_thread->shutdown(EXIT_FAILURE);

  // stop if in linting only mode
  if (!m_options.compile) m_thread->shutdown(EXIT_SUCCESS);
  else if (stats.hints) $::Debug::println(); // display padding

  // get an initial starting time-point
  auto start = $::Clock::Performance();

  // construct the necessary spinner now
  if (runtime->flags.verbose) m_spinner = async->spinner(s_suffix("Compiling Modules..."));

  // now we want to go through every file and archive them
  auto blob = m_archive->encode(modules);

  // declare as actually imbuing the executable now
  if (runtime->flags.verbose) m_spinner->suffix(s_suffix("Imbuing Executable..."));

  // get the underlying executable binary now and imbue it
  auto binary = XJCT::Archive::Binary($::Executable::resolve());
  if (!m_imbue(binary, blob)) m_thread->shutdown(EXIT_FAILURE);

  // finally show that we are writing out output
  if (runtime->flags.verbose) m_spinner->suffix(s_suffix("Writing Executable..."));

  // get the incoming output name to be used
  auto output = m_output(script, binary.extension());

  // attempt outputting the file with the desired options now
  $::FS::Overwrite(output, binary.buffer());

  // make the output file also executable now as well
  $::FS::Chmod.executable(output);

  // finally attempt code-signing if necessary
  if (!m_codesign(output)) m_thread->shutdown(EXIT_FAILURE);

  // immediately stop if not in verbose mode
  if (!runtime->flags.verbose) m_thread->shutdown(EXIT_SUCCESS);

  // prepare the elapsed time now
  auto details = fmt::format("Compiled '{0}' in {1}", output.filename().string(), $::Clock::Performance() - start);

  // finally we dismiss with the time elapsed and exit the worker now
  m_spinner->dismiss(s_suffix(details)), m_thread->shutdown(EXIT_SUCCESS);
}

$::URI::Buffer Sabre::Bundle::Worker::m_resolve(const $::String::View &script) {
  auto resource = resolve(script, $::System::cwd());
  if (resource.has_value()) return *resource;
  m_failure(8000000, resource.error());
  m_thread->shutdown(EXIT_FAILURE);
}

$::FS::Path Sabre::Bundle::Worker::m_output(const $::URI::View &script) {
  return m_output(script, XJCT::Archive::Extension);
}

$::FS::Path Sabre::Bundle::Worker::m_output(const $::URI::View &script, const $::String::View &extension) {
  auto canonical = m_options.output.size(); // prepare the baseline output to be used now based on output given
  auto output = canonical ? $::Path::canonical(m_options.output) : $::FS::Path(script.body()).replace_extension();

  // and handle deciding the correct extension to be used now
  return extension.size() && output.extension() != extension ? output += extension : output;
}

bool Sabre::Bundle::Worker::m_codesign(const $::FS::Path &output) const noexcept {
  return m_archive->provider()->codesign(output) || m_failure(9000902, $::Path::relative(output).string());
}

bool Sabre::Bundle::Worker::m_imbue(Executable &binary, const XJCT::Blob::Bytes &buffer) const noexcept {
  return m_imbue(binary, XJCT::Blob::View(reinterpret_cast<const char *>(buffer.data()), buffer.size()));
}

bool Sabre::Bundle::Worker::m_imbue(Executable &binary, const XJCT::Blob::View &blob) const noexcept {
  if (!m_archive->provider()->imbue(binary, {Fuse::BUNDLED.name(), blob})) return m_failure(9000900);
  return m_archive->provider()->infuse(binary, Fuse::BUNDLED) || m_failure(9000901, Fuse::BUNDLED.name());
}
