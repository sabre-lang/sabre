/// Sabre Includes
#include "sabre/dotenv/service.hpp"
#include "sabre/document/buffer.hpp"
#include "sabre/dotenv/reader.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/runtime/options.hpp"

//  PROPERTIES  //

static std::vector<$::String::View> g_envfiles = {
    ".env",
};

//  CONSTRUCTORS  //

Sabre::Dotenv::Service::Service() : Service($::Global::get<Runtime::Container>()) {}
Sabre::Dotenv::Service::Service(XI::Container *services) : m_services(services) {
  // prepare a common base location to be used
  auto hint = $::System::cwd();

  // prepare the environment files to be read from initially
  const auto &envfiles = m_services->get<Runtime::Options>()->script.dotenv;

  // attempt parsing each of the available environment files now
  for (const auto &relative : envfiles) m_parse(m_resolve(relative, hint));
}

//  PRIVATE METHODS  //

void Sabre::Dotenv::Service::m_parse(const $::FS::Path &envfile) noexcept {
  // ignore if the file does not exist
  if (envfile.empty()) return;

  // should be able to construct a buffer of the file in question
  auto buffer = Document::Buffer(envfile);

  // construct a reader to parse the given envfile
  m_variables = Reader::parse(buffer.view(), std::move(m_variables));
}

$::FS::Path Sabre::Dotenv::Service::m_resolve(const $::String::View &relative, const $::FS::Path &hint) const noexcept {
  // get the current verbosity being used
  auto verbose = m_services->get<Runtime::Options>()->flags.verbose;

  // if we have been given a relative path, then find canonical one
  if (!relative.empty()) {
    // ensure the incoming path given is valid
    auto canonical = $::Path::canonical(relative, hint);
    if ($::Path::exists(canonical)) return canonical;

    // since invalid then we actually warn user of this
    return m_warn(verbose, "Envfile '{0}' does not exist", relative), $::FS::Path();
  }

  // otherwise search through common environment file locations
  for (const auto &basename : g_envfiles) {
    // attempt finding a suitable path to be used
    auto path = $::Path::join(hint, basename);
    if (!$::Path::exists(path)) continue;

    // found an environment file so alert user
    return m_warn(verbose, "Found default envfile '{0}'", basename), path;
  }

  // warn about not finding a suitable automatic environment file
  return m_warn(verbose, "Could not find a default envfile"), $::FS::Path();
}
