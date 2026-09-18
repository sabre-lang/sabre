/// Sabre Includes
#include <sabre/toolchain/serve.hpp>

/// Shell Includes
#include "shell/command/macros.hpp"
#include "shell/serve/action.hpp"

//  X-MACROS  //

#define XX_OPTIONS_LIST(X)                            \
  X("--stdio", "Ensures using standard IO transport") \
  SHELL_XX_OPTIONS_COMMON(X)

//  CONSTRUCTORS  //

Shell::Serve::Action::Action() : Abstract("serve") {
#define X(N, D, ...) {$::Color::ANSI(N), D},
  m_descriptor.options({XX_OPTIONS_LIST(X)});
#undef X

  // forcing linting to be enabled for language-servers
  m_runtime.flags.lint = true;

  // update some of the initial options now
  m_runtime.async.worker.wait_time = 1s;

  // ensure we show all diagnostic values as well
  m_runtime.diagnostics.severity = Sabre::Diagnostic::Severity::HINT;
}

//  PRIVATE METHODS  //

void Shell::Serve::Action::m_execute() const {
  auto exit_code = Sabre::Toolchain::serve(m_runtime);
  if (exit_code) throw CLI::RuntimeError(exit_code);
}

void Shell::Serve::Action::m_subscribe(CLI::App *command) {
  // bind all the common runtime options
  m_common(command, &m_runtime, false);

  // ignore the incoming "--stdio" flag (as it should be set)
  command->add_flag("--stdio")->required(true);

  // set the necessary callback to run the instance now
  command->callback(std::bind(&Action::m_execute, this));
}
