/// Sabre Includes
#include "sabre/toolchain/test.hpp"
#include "sabre/testing/service.hpp"
#include "sabre/testing/storage.hpp"
#include "sabre/toolchain/launch.hpp"

//  PUBLIC METHODS  //

int32_t Sabre::Toolchain::test(const Runtime::Options &options) {
  return bootstrap(options)->load<Testing::Service>()->spawn();
}
