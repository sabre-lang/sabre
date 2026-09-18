/// Sabre Includes
#include "sabre/module/abstract.hpp"
#include "sabre/module/dynamic.hpp"
#include "sabre/module/script.hpp"
#include "sabre/runtime/options.hpp"

/// Metadata Includes
#include "sabre/bytecode/metadata.hpp"
#include "sabre/syntax/metadata.hpp"
#include "sabre/type/metadata.hpp"

//  X-MACROS  //

#define XX_MODULE_DUMPS(X)        \
  X(SYNTAX, syntax, PARSED)       \
  X(TYPEDEFS, types, TYPED)       \
  X(BYTECODE, bytecode, COMPILED)

#define X(D, N, P, ...) $_FWD(Sabre::Module::Print, void N(const Abstract *, const Metadata::Wrapper<Phase::P> *))
XX_MODULE_DUMPS(X)
#undef X

//  PUBLIC METHODS  //

void Sabre::Module::Print::syntax(const Abstract *self, const Metadata::Wrapper<Phase::PARSED> *) {
  // ignore when we have non-script values given
  if (!self->is<Script>()) return;

  // show the baseline dump details now
  $::Debug::println("\n===== Abstract Syntax Tree '{0}' =====\n", self->resource().relative().string());

  /// TODO: show all the syntax for this module
  $_ABORT("Unimplemented 'Syntax Dump'");
}

void Sabre::Module::Print::types(const Abstract *self, const Metadata::Wrapper<Phase::TYPED> *) {
  // ignore when we have non-script values given
  if (!self->is<Script>()) return;

  // show the baseline dump details now
  $::Debug::println("\n===== Type Definitions '{0}' =====\n", self->resource().relative().string());

  /// TODO: show all the syntax for this module
  $_ABORT("Unimplemented 'Types Dump'");
}

void Sabre::Module::Print::bytecode(const Abstract *self, const Metadata::Wrapper<Phase::COMPILED> *metadata) {
  // ignore when we have non-compiled modules
  if (self->is<Dynamic>()) return;

  // show the baseline dump details now
  $::Debug::println("\n===== Bytecode Dump '{0}' =====\n", self->resource().relative().string());

  // print the arena if it has some functions
  if (metadata->arena()->functions.size()) $::Debug::println("{0}\n", *metadata->arena());
}

//  PRIVATE METHODS  //

void Sabre::Module::Abstract::m_dump(Dump type, XI::Container *services) const noexcept {
#define X(D, N, P, ...)                                        \
  case Dump::D: {                                              \
    if (options->dump.N) Print::N(this, metadata<Phase::P>()); \
  } break;

  switch (Runtime::Options *options = *services; type) { XX_MODULE_DUMPS(X) default : break; }
#undef X
}
