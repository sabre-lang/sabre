/// Sabre Includes
#include <sabre/bundle/fuse.hpp>
#include <sabre/toolchain/launch.hpp>

/// Shell Includes
#include "shell/command/program.hpp"

/// Action Includes
#include "shell/bundle/action.hpp"
#include "shell/format/action.hpp"
#include "shell/launch/action.hpp"
#include "shell/lint/action.hpp"
#include "shell/serve/action.hpp"
#include "shell/task/action.hpp"
#include "shell/test/action.hpp"
#include "shell/upgrade/action.hpp"

/**
 * @brief Sabre runtime entry-point.
 * @param argc                  Argument count.
 * @param argv                  Variadic arguments.
 */
int32_t main(int32_t argc, char **argv) {
  // ensure the underlying terminal can use utf-8
  $::Terminal::utf8();

  // check if we have a bundled program to bypass
  if (Sabre::Fuse::BUNDLED.exists()) return Sabre::Toolchain::launch(argc, argv);

  // construct the underlying program
  auto program = Shell::Command::Program();

  // subscribe all the available actions now
  program.subscribe<Shell::Launch::Action>();
  program.subscribe<Shell::Task::Action>();
  program.subscribe<Shell::Test::Action>();

  program.subscribe<Shell::Bundle::Action>();
  program.subscribe<Shell::Format::Action>();
  program.subscribe<Shell::Lint::Action>();
  program.subscribe<Shell::Serve::Action>();

  program.subscribe<Shell::Upgrade::Action>();

  // and finally attempt launching the program
  return program.launch(argc, argv);
}
