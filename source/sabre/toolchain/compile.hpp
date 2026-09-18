#ifndef _SABRE_TOOLCHAIN_COMPILE_HPP
#define _SABRE_TOOLCHAIN_COMPILE_HPP

/// Sabre Modules
#include "sabre/image/arena.hpp"
#include "sabre/syntax/tree.hpp"
#include "sabre/variable/captures.hpp"

namespace Sabre::Toolchain {

/**
 * @brief Handles compiling a module.
 * @param syntax                Syntax tree.
 * @param services              Runtime services.
 */
$::Unique::Pointer<Image::Arena> compile(const Syntax::Tree *syntax, XI::Container *services);

} // namespace Sabre::Toolchain

#endif
