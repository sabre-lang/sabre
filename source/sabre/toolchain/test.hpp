#ifndef _SABRE_TOOLCHAIN_TEST_HPP
#define _SABRE_TOOLCHAIN_TEST_HPP

/// Sabre Includes
#include "sabre/runtime/options.hpp"

namespace Sabre::Toolchain {

/**
 * @brief Handles launching a testing runtime.
 * @param options                           Runtime options.
 */
int32_t test(const Runtime::Options &options = {});

} // namespace Sabre::Toolchain

#endif
