/// Sabre Includes
#include "sabre/toolchain/bundle.hpp"
#include "sabre/bundle/service.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/toolchain/launch.hpp"

//  PUBLIC METHODS  //

int32_t Sabre::Toolchain::lint(const Runtime::Options &runtime) {
  return bundle(Bundle::Options(Bundle::Mode::LINT), runtime);
}

int32_t Sabre::Toolchain::docify(const Author::Options &options, const Runtime::Options &runtime) {
  return bundle(Bundle::Options(options), runtime);
}

int32_t Sabre::Toolchain::bundle(const Bundle::Options &options, const Runtime::Options &runtime) {
  return bootstrap(runtime)->load<Bundle::Service>()->bundle(options);
}
