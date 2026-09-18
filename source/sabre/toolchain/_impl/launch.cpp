/// Sabre Includes
#include "sabre/toolchain/launch.hpp"
#include "sabre/bundle/archive.hpp"
#include "sabre/bundle/fuse.hpp"
#include "sabre/import/service.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/runtime/entry.hpp"

/// Forward Definitions
$_FWD(Sabre::Toolchain, int32_t launch(XI::Container *))

//  PUBLIC METHODS  //

XI::Container *Sabre::Toolchain::bootstrap(const Runtime::Options &options) {
  return $::Global::get<Runtime::Container>(options);
}

int32_t Sabre::Toolchain::launch(XI::Container *services) {
  return services->get<Async::Service>()->launch<Runtime::Entry>(services);
}

int32_t Sabre::Toolchain::launch(const Runtime::Options &options) { return launch(bootstrap(options)); }
int32_t Sabre::Toolchain::launch(int32_t argc, char **argv) {
  // prepare the archive and options
  auto archive = Bundle::Archive();
  auto options = Runtime::Options();

  // we want to decode the incoming filesystem
  auto filesystem = archive.decode(Fuse::BUNDLED);

  // prepare the baseline bundled options
  options.flags.typeless = true;

  // prepare the script entry to be used from the filesystem
  options.script.argv = {argv, argv + argc};
  options.script.entry = filesystem.resources.front().view();

  // bootstrap the runtime options now
  auto *services = bootstrap(options);
  Import::Service *modules = *services;

  // ensure we preload the archived modules
  modules->preload(std::move(filesystem.scripts));

  // and attempt launching the runtime now
  return launch(services);
}
