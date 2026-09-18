/// Shell Includes
#include "shell/command/abstract.hpp"

//  PRIVATE METHODS  //

void Shell::Command::Abstract::m_common(CLI::App *command, Sabre::Runtime::Options *options, bool runtime) {
  // prepare some common environment/internal items now
  auto &stack_size = options->async.thread.stack_size;
  auto &stack_limit = options->async.thread.stack_limit;
  auto &hardware_limit = options->async.vprocs.hardware_limit;

  // prepare all the hidden command line options
  command->add_option("--internal-stack-size", stack_size)->envname("SABRE_STACK_SIZE");
  command->add_option("--internal-stack-limit", stack_limit)->envname("SABRE_STACK_LIMIT");
  command->add_option("--internal-hardware-vprocs", hardware_limit)->envname("SABRE_HARDWARE_VPROCS");

  // if not if runtime mode, then stop handling
  if (!runtime) return;

  // add the linting flag to be used now
  command->add_flag("--lint", options->flags.lint);

  auto hints = [severity = &options->diagnostics.severity] { *severity = Sabre::Diagnostic::Severity::HINT; };
  command->add_flag_callback("--hints", hints); // allows showing additional linting details where possible

  // prepare the baseline runtime flags
  command->add_flag("--optless", options->flags.optless);
  command->add_flag("--jitless", options->flags.jitless);
  command->add_flag("--typeless", options->flags.typeless);

  // and the dump-based flags to be inherited
  command->add_flag("--dump-syntax", options->dump.syntax);
  command->add_flag("--dump-types", options->dump.types);
  command->add_flag("--dump-graphs", options->dump.graphs);
  command->add_flag("--dump-bytecode", options->dump.bytecode);
  command->add_flag("--dump-assembly", options->dump.assembly);
}
