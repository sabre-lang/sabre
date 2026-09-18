/// Sabre Includes
#include "sabre/module/script.hpp"
#include "sabre/document/service.hpp"
#include "sabre/runtime/options.hpp"
#include "sabre/toolchain/audit.hpp"
#include "sabre/toolchain/compile.hpp"
#include "sabre/toolchain/parse.hpp"

/// Metadata Includes
#include "sabre/bytecode/metadata.hpp"
#include "sabre/syntax/metadata.hpp"
#include "sabre/type/metadata.hpp"

//  PRIVATE METHODS  //

void Sabre::Module::Script::m_parse(XI::Container *services) {
  // resolve the document and details
  auto document = m_document(services);
  auto *syntax = metadata<Phase::PARSED>();

  // get the outgoing diagnostics encoding
  Runtime::Options *runtime = *services;

  // prepare the baseline options for the lexer
  Lexer::Options options = {
      .comments = runtime->flags.lint,
      .encoding = runtime->diagnostics.encoding,
      .reporter = reporter(), // use the reporter
  };

  // bind the necessary tokens and tree for the syntax
  syntax->tokens() = Toolchain::lex(*document.get(), options);
  syntax->tree() = Toolchain::parse(&syntax->tokens(), reporter());
}

void Sabre::Module::Script::m_analyze(XI::Container *services) {
  // resolve the required metadata now
  auto *types = metadata<Phase::TYPED>();
  auto *syntax = metadata<Phase::PARSED>()->tree().get();

  // get the incoming runtime options now
  auto *options = services->get<Runtime::Options>();

  // determine if we have a typeless instance to be used
  if (options->flags.typeless) return;

  // resolve the suitable context instance now (if actually allowing typing)
  types->exports() = Toolchain::audit(syntax, services, reporter());

  // only allow linting if necessary to do so
  if (options->flags.lint) types->mirrors() = Toolchain::lint(syntax, services, reporter());
}

void Sabre::Module::Script::m_compile(XI::Container *services) {
  auto *syntax = metadata<Phase::PARSED>()->tree().get(); // prepare syntax
  metadata<Phase::COMPILED>()->arena() = Toolchain::compile(syntax, services);
}
