#ifndef _SABRE_TOOLCHAIN_BUNDLE_HPP
#define _SABRE_TOOLCHAIN_BUNDLE_HPP

/// Sabre Includes
#include "sabre/bundle/options.hpp"
#include "sabre/runtime/options.hpp"

namespace Sabre::Toolchain {

/**
 * @brief Handles linting a runtime.
 * @param options               Runtime options.
 */
int32_t lint(const Runtime::Options &options = {});

/**
 * @brief Handles bundling a runtime.
 * @param options               Options to bundle.
 * @param runtime               Runtime options.
 */
int32_t bundle(const Bundle::Options &options, const Runtime::Options &runtime = {});

} // namespace Sabre::Toolchain

#endif
