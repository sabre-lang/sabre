/// Sabre Includes
#include <sabre/toolchain/test.hpp>

/// Shell Includes
#include "shell/command/macros.hpp"
#include "shell/test/action.hpp"
#include "shell/validators/duration.hpp"

//  X-MACROS  //

#define XX_OPTIONS_LIST(X)                                                 \
  X("--pattern", "Globbing pattern for matching tests")                    \
  X("--bailout", "Exit the testing suite after <number> failures")         \
  X("--timeout", "Per-test timeout in milliseconds")                       \
  X("", "")                                                                \
  X("--seed", "Sets the seed for test randomization")                      \
  X("--randomize", "Runs all tests in a random order")                     \
  X("", "")                                                                \
  X("--warmup", "Total warmup runs before running a benchmark")            \
  X("--samples", "Denotes the maximum benchmark samples to take")          \
  X("--benchmarks", "Enables benchmark testing cases to be evaluated")     \
  X("", "")                                                                \
  X("--quiet", "Hides verbose testing outputs, only for 'text' reporters") \
  X("--outfile", "Reporter output file. Defaults to console output")       \
  X("--reporter", "The reporter output format. Defaults to 'text'")        \
  X("", "")                                                                \
  SHELL_XX_OPTIONS_LAUNCH(X)                                               \
  X("", "")                                                                \
  SHELL_XX_OPTIONS_COMMON(X)

//  CONSTRUCTORS  //

Shell::Test::Action::Action() : Abstract("test") {
#define X(N, D, ...) {$::Color::ANSI(N), D},
  m_descriptor.options({XX_OPTIONS_LIST(X)});
#undef X

  // forcibly enable verbose actions
  m_runtime.flags.verbose = true;
}

//  PRIVATE METHODS  //

void Shell::Test::Action::m_execute() const {
  auto exit_code = Sabre::Toolchain::test(m_runtime);
  if (exit_code) throw CLI::RuntimeError(exit_code);
}

void Shell::Test::Action::m_subscribe(CLI::App *command) {
  // we allow all positionals to get passed through
  command->positionals_at_end(true);

  // bind all the runtime options to be used
  m_common(command, &m_runtime, true);

  // prepare all the baseline flags to be used
  command->add_flag("--randomize", m_runtime.testing.randomize);
  command->add_flag("--benchmarks", m_runtime.testing.bench.enabled);

  // prepare all the necessary testing options
  command->add_option("--pattern", m_runtime.testing.pattern);
  command->add_option("--bailout", m_runtime.testing.bailout);
  command->add_option("--warmup", m_runtime.testing.bench.warmup);
  command->add_option("--samples", m_runtime.testing.bench.samples.maximum);

  // prepare the verbosing flag now
  command->add_flag_callback("--quiet", [&] { m_runtime.flags.verbose = false; });

  // duration values require a little more involved setup
  command->add_option<$::Chrono::Duration, size_t>("--timeout", m_runtime.testing.bench.timeout)
      ->transform(Validator::Duration());

  // set the necessary callback to run the instance now
  command->callback(std::bind(&Action::m_execute, this));
}
