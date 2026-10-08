/// Sabre Includes
#include <sabre/toolchain/bundle.hpp>

/// Shell Includes
#include "shell/command/macros.hpp"
#include "shell/docify/action.hpp"

//  X-MACROS  //

#define XX_OPTIONS_LIST(X)                                    \
  X("--quiet", "Hides verbose docification outputs")          \
  X("--outfile", "The output file to write documentation to") \
  X("", "")                                                   \
  SHELL_XX_OPTIONS_COMMON(X)

//  CONSTRUCTORS  //

Shell::Docify::Action::Action() : Abstract("docify") {
#define X(N, D, ...) {$::Color::ANSI(N), D},
  m_descriptor.positionals({{"script", false}}).options({XX_OPTIONS_LIST(X)});
#undef X

  // forcibly enable linting to occur
  m_runtime.flags.lint = true;
  m_runtime.flags.verbose = true;

  // ensure linting ONLY shows us errors received
  m_runtime.diagnostics.severity = Sabre::Diagnostic::Severity::ERROR;
}

//  PRIVATE METHODS  //

void Shell::Docify::Action::m_execute() {
  auto exit_code = Sabre::Toolchain::docify(m_options, m_runtime);
  if (exit_code) throw CLI::RuntimeError(exit_code);
}

void Shell::Docify::Action::m_subscribe(CLI::App *command) {
  // we allow all positionals to get passed through
  command->positionals_at_end(true);

  // bind all the common options
  m_common(command, &m_runtime, false);

  // prepare some additional options now
  command->add_option("--outfile", m_options.output);

  // prepare the verbosity flag now
  command->add_flag_callback("--quiet", [&] { m_runtime.flags.verbose = false; });

  // prepare the positionsal the will be available now
  command->add_option("script.sabre", m_runtime.script);

  // set the necessary callback to run the instance now
  command->callback(std::bind(&Action::m_execute, this));
}
