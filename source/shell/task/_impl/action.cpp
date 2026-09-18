/// Sabre Includes
#include <sabre/crate/registry.hpp>

/// Shell Includes
#include "shell/command/macros.hpp"
#include "shell/task/action.hpp"

//  X-MACROS  //

#define XX_OPTIONS_LIST(X)                                                          \
  X("-p, --par, --parallel", "Execute following tasks in parallel")                 \
  X("-s, --seq, --sequential", "Execute following tasks in sequence (default)")     \
  X("", "")                                                                         \
  X("-q, --quiet", "Disables verbose task diagnostic outputs")                      \
  X("-v, --verbose", "Enables verbose task diagnostic outputs (default)")           \
  X("-f, --filter", "Filters the workspace(s) in which to find and execute a task") \
  X("", "")                                                                         \
  SHELL_XX_OPTIONS_COMMON(X)

//  CONSTRUCTORS  //

Shell::Task::Action::Action() : Abstract("task") {
#define X(N, D, ...) {$::Color::ANSI(N), D},
  m_descriptor.options({XX_OPTIONS_LIST(X)});
#undef X
}

//  PRIVATE METHODS  //

void Shell::Task::Action::m_execute(CLI::App *command) const {
  // prepare the incoming arguments to be resolved
  auto argv = command->remaining();

  // prepare the incoming passthrough arguments as well
  $_UNUSED $_AUTO = command->remaining_for_passthrough();

  // iterate over the arguments to resolve branches and delegate them after
  for (Branch state = {}; argv.size(); m_delegate(state)) m_branch(state, argv);
}

void Shell::Task::Action::m_delegate(const Branch &branch) const {
  // attempt resolving all the workspaces (via patterns)
  auto workspaces = branch.workspaces();
  auto futures = std::vector<std::future<void>>();

  // if the tasks or workspaces are empty, so we stop early
  if (workspaces.empty()) return m_warn(branch.verbose(), "No workspaces provided");
  if (branch.empty()) return m_warn(branch.verbose(), "No script names provided");

  /// TODO: execute the workspaces in parallel (currently sequential)
  for (const auto &workspace : workspaces) {
    auto crate = Sabre::Crate::Registry::view(workspace); // view
    if (crate == nullptr || branch.execute(crate.get())) continue;
    throw CLI::RuntimeError(EXIT_FAILURE); // this is a failure
  }
}

void Shell::Task::Action::m_subscribe(CLI::App *command) {
  // force all items to be available
  command->allow_extras(true);

  // set the necessary callback to run the instance now
  command->callback(std::bind(&Action::m_execute, this, command));
}

void Shell::Task::Action::m_branch(Branch &state, Collection &argv) const {
  // pre-clear the incoming tasks now being handled
  state.m_tasks.clear();

  // check for conditions to actually exit as well
  auto update = [&](auto &ref, const auto &value) -> bool { return state.empty() ? (ref = value, true) : false; };

#define MM_UPDATE(N, R, V)     \
  case XH::FNV::U32(N): {      \
    if (!update(R, V)) return; \
  } break

  // iterate over the incoming arguments now as necessary
  for (; argv.size(); argv.erase(argv.begin())) {
    // get the current front-most item now
    auto front = argv.front();

    // handle the incoming details as necessary
    switch (XH::FNV::U32(front)) {
      MM_UPDATE("-p", state.m_parallel, true);
      MM_UPDATE("--par", state.m_parallel, true);
      MM_UPDATE("--parallel", state.m_parallel, true);

      MM_UPDATE("-s", state.m_parallel, false);
      MM_UPDATE("--seq", state.m_parallel, false);
      MM_UPDATE("--sequential", state.m_parallel, false);

      MM_UPDATE("-q", state.m_verbose, false);
      MM_UPDATE("--quiet", state.m_verbose, false);

      MM_UPDATE("-v", state.m_verbose, true);
      MM_UPDATE("--verbose", state.m_verbose, true);

    case XH::FNV::U32("-f"): $_FALLTHROUGH;
    case XH::FNV::U32("--filter"): {
      if (state.m_tasks.size()) return; // should ignore til next
      if (argv.size() == 1) m_fail("The '{0}' flag requires a value", front);
      argv.erase(argv.begin()), state.m_filter = argv.front(); // set now
    } break;

    // handle incoming regular values as potential "tasks"
    default: state.m_tasks.emplace_back(front); break;
    }
  }

#undef MM_UPDATE
}
