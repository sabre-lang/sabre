/// Sabre Includes
#include "sabre/toolchain/audit.hpp"
#include "sabre/relint/analyzer.hpp"
#include "sabre/type/analyzer.hpp"

//  PUBLIC METHODS  //

$::Unique::Pointer<Sabre::Type::Exports>
Sabre::Toolchain::audit(const Syntax::Tree *syntax, XI::Container *services, Diagnostic::Reporter *reporter) {
  return services->get<Type::Analyzer>()->audit(syntax, reporter);
}

$::Unique::Pointer<Sabre::Relint::Exports>
Sabre::Toolchain::lint(const Syntax::Tree *syntax, XI::Container *services, Diagnostic::Reporter *reporter) {
  return services->get<Relint::Analyzer>()->audit(syntax, reporter);
}
