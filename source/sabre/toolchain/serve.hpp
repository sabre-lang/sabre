#ifndef _SABRE_TOOLCHAIN_SERVE_HPP
#define _SABRE_TOOLCHAIN_SERVE_HPP

/// Sabre Includes
#include "sabre/runtime/options.hpp"

namespace Sabre::Toolchain {

/**
 * @brief Handles launching the "sabre" language-server.
 * @param options                   Options to bind.
 */
int32_t serve(const Runtime::Options &options = {});

} // namespace Sabre::Toolchain

#endif
