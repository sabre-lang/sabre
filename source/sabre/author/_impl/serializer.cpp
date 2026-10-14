/// Sabre Includes
#include "sabre/author/serializer.hpp"
#include "sabre/async/service.hpp"
#include "sabre/runtime/container.hpp"

/// Serializer Includes
#include "sabre/author/_inline/json.ipp"

//  CONSTRUCTORS  //

Sabre::Author::Serializer::Serializer(const Docify *self) : Serializer($::Global::get<Runtime::Container>(), self) {}
Sabre::Author::Serializer::Serializer(XI::Container *services, const Docify *self) : m_services(services) {
  switch (self->options().format) {
  default: $_FALLTHROUGH; // always default to using the JSON as output
  case Format::JSON: m_proxy = $::Unique::New<Proxy<Format::JSON>>(self);
  }
}

//  PUBLIC METHODS  //

void Sabre::Author::Serializer::write() {
  // prepare a suitable spinner suffix to be used
  static auto s_suffix = $::Spinner::Suffix("Docifyer");

  // get the runtime details to be used now
  Async::Service *async = *m_services;
  Runtime::Options *runtime = *m_services;

  // prepare a spinner if necessary to do so
  if (runtime->flags.verbose) m_spinner = async->spinner(s_suffix("Rendering Modules..."));

  // get an initial starting time-point
  auto start = $::Clock::Performance();

  // prepare our serialization output details to be used
  auto content = m_proxy->render();

  // check if requested to output to console instead
  m_proxy->options()->console ? $::Debug::println(content) : $::FS::Overwrite(m_outfile(), content);

  // stop early if not executing verbose outputs
  if (!runtime->flags.verbose) return;

  auto elapsed = $::Clock::Performance() - start; // prepare the elapsed time
  auto details = fmt::format("Rendered '{0}' output in {1}", m_proxy->label(), elapsed);
  m_spinner->dismiss(s_suffix(details)); // on completion show the final details
}

//  PRIVATE METHODS  //

$::String::Buffer Sabre::Author::Serializer::m_outfile() const noexcept {
  // if the base input is empty, then resolve use the current working directory
  auto hint = $::URI::File($::Path::current() / "_docify");
  return Outfile(m_proxy->options()->output, hint, m_proxy->extension());
}
