/// Sabre Includes
#include "sabre/toolchain/serve.hpp"
#include "sabre/server/connection.hpp"
#include "sabre/toolchain/launch.hpp"

//  PUBLIC METHODS  //

int32_t Sabre::Toolchain::serve(const Runtime::Options &options) {
  return bootstrap(options)->load<Server::Connection>()->listen();
}
