#ifndef _SABRE_TOOLCHAIN_LAUNCH_HPP
#define _SABRE_TOOLCHAIN_LAUNCH_HPP

/// Sabre Includes
#include "sabre/runtime/options.hpp"

namespace Sabre::Toolchain {

/**
 * @brief Handles launching a bundled runtime.
 * @param argc                      Argument count.
 * @param argv                      Variadic arguments.
 */
int32_t launch(int32_t argc, char **argv);

/**
 * @brief Handles launching a development runtime.
 * @param options                   Runtime options.
 */
int32_t launch(const Runtime::Options &options = {});

/**
 * @brief Bootstraps a runtime container.
 * @param options                   Runtime options.
 */
XI::Container *bootstrap(const Runtime::Options &options = {});

} // namespace Sabre::Toolchain

#endif
