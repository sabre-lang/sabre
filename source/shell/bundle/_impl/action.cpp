/// Sabre Includes
#include <sabre/toolchain/bundle.hpp>

/// Shell Includes
#include "shell/bundle/action.hpp"
#include "shell/command/macros.hpp"

//  X-MACROS  //

#define XX_OPTIONS_LIST(X)                                     \
  X("--quiet", "Hides verbose compilation outputs")            \
  X("--outfile", "The output file to write the executable to") \
  X("", "")                                                    \
  SHELL_XX_OPTIONS_BUNDLE(X)                                   \
  X("", "")                                                    \
  SHELL_XX_OPTIONS_COMMON(X)

//  CONSTRUCTORS  //

Shell::Bundle::Action::Action() : Abstract("bundle") {
  // emplace all the available usage details
#define X(N, D, ...) {$::Color::ANSI(N), D},
  m_descriptor.positionals({{"script", false}}).options({XX_OPTIONS_LIST(X)});
#undef X

  // update some of the underlying options as well
  m_runtime.flags.jitless = true;
  m_runtime.flags.verbose = true;
}

//  PRIVATE METHODS  //

void Shell::Bundle::Action::m_execute() {
  // get some details about the arguments
  auto begin = m_runtime.script.argv.begin();
  auto end = m_runtime.script.argv.end();
  auto dashes = std::find(begin, end, "--");

  // remove any items before and including the dashes
  if (dashes != end) m_runtime.script.argv.erase(begin, dashes + 1);

  auto exit_code = Sabre::Toolchain::bundle(m_options, m_runtime);
  if (exit_code) throw CLI::RuntimeError(exit_code); // failed
}

void Shell::Bundle::Action::m_subscribe(CLI::App *command) {
  // we allow all positionals to get passed through
  command->positionals_at_end(true);

  // bind all the common options
  m_common(command, &m_runtime, true);

  // prepare some additional options now
  command->add_option("--outfile", m_options.output);

  // prepare the verbosity flag now
  command->add_flag_callback("--quiet", [&] { m_runtime.flags.verbose = false; });

  // prepare the positionsal the will be available now
  command->add_option("script.sabre", m_runtime.script);
  command->add_option("argv", m_runtime.script.argv);

  // set the necessary callback to run the instance now
  command->callback(std::bind(&Action::m_execute, this));
}
