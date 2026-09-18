/// Sabre Includes
#include <sabre/toolchain/bundle.hpp>

/// Shell Includes
#include "shell/command/macros.hpp"
#include "shell/lint/action.hpp"

//  X-MACROS  //

#define XX_OPTIONS_LIST(X)                           \
  X("--quiet", "Hides verbose linting outputs")      \
  X("--hints", "Allows showing extra linting hints") \
  X("", "")                                          \
  SHELL_XX_OPTIONS_COMMON(X)

//  CONSTRUCTORS  //

Shell::Lint::Action::Action() : Abstract("lint") {
#define X(N, D, ...) {$::Color::ANSI(N), D},
  m_descriptor.positionals({{"script", false}}).options({XX_OPTIONS_LIST(X)});
#undef X

  // forcibly enable linting to occur
  m_runtime.flags.lint = true;
  m_runtime.flags.verbose = true;
}

//  PRIVATE METHODS  //

void Shell::Lint::Action::m_execute() const {
  auto exit_code = Sabre::Toolchain::lint(m_runtime);
  if (exit_code) throw CLI::RuntimeError(exit_code);
}

void Shell::Lint::Action::m_subscribe(CLI::App *command) {
  // we allow all positionals to get passed through
  command->positionals_at_end(true);

  // bind all the common options
  m_common(command, &m_runtime, false);

  // prepare the callback flags to be used now
  command->add_flag_callback("--quiet", [&] { m_runtime.flags.verbose = false; });
  command->add_flag_callback("--hints", [&] { m_runtime.diagnostics.severity = Sabre::Diagnostic::Severity::HINT; });

  // prepare the positionsal the will be available now
  command->add_option("script.sabre", m_runtime.script);

  // set the necessary callback to run the instance now
  command->callback(std::bind(&Action::m_execute, this));
}
