#ifndef _SABRE_TOOLCHAIN_AUDIT_HPP
#define _SABRE_TOOLCHAIN_AUDIT_HPP

/// Sabre Includes
#include "sabre/diagnostic/reporter.hpp"
#include "sabre/relint/exports.hpp"
#include "sabre/syntax/tree.hpp"
#include "sabre/type/exports.hpp"

namespace Sabre::Toolchain {

/**
 * @brief Handles analyzing a syntax-tree.
 * @param syntax                Syntax tree.
 * @param services              Services container.
 * @param reporter              Diagnostic reporter.
 */
$::Unique::Pointer<Type::Exports>
audit(const Syntax::Tree *syntax, XI::Container *services, Diagnostic::Reporter *reporter);

/**
 * @brief Handles linting a syntax-tree.
 * @param syntax                Syntax tree.
 * @param services              Services container.
 * @param reporter              Diagnostic reporter.
 */
$::Unique::Pointer<Relint::Exports>
lint(const Syntax::Tree *syntax, XI::Container *services, Diagnostic::Reporter *reporter);

} // namespace Sabre::Toolchain

#endif
