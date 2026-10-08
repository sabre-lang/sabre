/// Sabre Includes
#include "sabre/bundle/worker.hpp"
#include "sabre/bundle/fuse.hpp"
#include "sabre/bundle/service.hpp"
#include "sabre/document/buffer.hpp"
#include "sabre/import/service.hpp"

//  CONSTRUCTORS  //

Sabre::Bundle::Worker::Worker(const Options &options) : Entry(), m_options(options) {}
Sabre::Bundle::Worker::Worker(XI::Container *services, const Options &options) :
    Entry(services), m_options(options), m_archive(*services) {}

//  PRIVATE METHODS  //

$_NORETURN void Sabre::Bundle::Worker::m_execute() { m_thread->shutdown(m_bundle()); }
int32_t Sabre::Bundle::Worker::m_bundle() {
  // prepare a suitable spinner suffix to be used
  static auto s_suffix = $::Spinner::Suffix("Bundling");

  // ensure suitable scoped for the execution
  m_scope();

  // prepare the necessary services to be used
  auto *async = service<Async::Service>();
  auto *runtime = service<Runtime::Options>();
  auto *modules = service<Import::Service>();

  // start by resolving the underlying script
  auto script = m_resolve(runtime->script.entry);
  if (!script.has_value()) return EXIT_FAILURE;

  // prepare a reporter to be used as well
  $::Unique::Pointer<Diagnostic::Reporter> reporter = *m_services;

  // attempt running analysis now
  auto stats = runtime->flags.typeless ? Import::Statistics() : modules->analyze(*script, reporter.get(), true);

  // attempt checking the types available now as necessary
  if (stats.errors) return EXIT_FAILURE;

  // handle the current state as necessary now
  switch (m_options.mode) {
  default: break;
  case Mode::LINT: return EXIT_SUCCESS;
  case Mode::DOCS: return m_docify(*script);
  }

  // show padding if given some hints at all
  if (stats.hints) $::Debug::println();

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
  if (!m_imbue(binary, blob)) return EXIT_FAILURE;

  // finally show that we are writing out output
  if (runtime->flags.verbose) m_spinner->suffix(s_suffix("Writing Executable..."));

  // get the incoming output name to be used
  auto output = m_output(*script, binary.extension());

  // attempt outputting the file with the desired options now
  $::FS::Overwrite(output, binary.span());

  // make the output file also executable now as well
  $::FS::Chmod.executable(output);

  // finally attempt code-signing if necessary
  if (!m_codesign(output)) return EXIT_FAILURE;

  // immediately stop if not in verbose mode
  if (!runtime->flags.verbose) return EXIT_SUCCESS;

  // prepare the elapsed time now
  auto details = fmt::format("Compiled '{0}' in {1}", output.filename().string(), $::Clock::Performance() - start);

  // finally we dismiss with the time elapsed and exit the worker now
  return m_spinner->dismiss(s_suffix(details)), EXIT_SUCCESS;
}

std::optional<$::URI::Buffer> Sabre::Bundle::Worker::m_resolve(const $::String::View &script) {
  auto resource = resolve(script, $::System::cwd());
  if (resource.has_value()) return *resource;
  return m_failure(8000000, resource.error()), std::nullopt;
}

int32_t Sabre::Bundle::Worker::m_docify(const $::URI::View &) { return EXIT_SUCCESS; }

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
