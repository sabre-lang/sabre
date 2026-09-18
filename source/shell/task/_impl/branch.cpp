/// Sabre Includes
#include <sabre/crate/constants.hpp>

/// Shell Includes
#include "shell/task/action.hpp"

//  PUBLIC METHODS  //

bool Shell::Task::Branch::execute(const Sabre::Crate::Manifest *crate) const {
  return m_parallel ? m_concurrent(crate) : m_sequential(crate);
}

Shell::Task::Collection Shell::Task::Branch::workspaces() const noexcept {
  // prepare the constant to be used now
  static constexpr auto s_crate = Sabre::Crate::Constants::filename();

  // if the workspace given is empty, then resolve to this current directory
  if (m_filter.empty() || m_filter == ".") {
    auto base = $::Path::current() / s_crate;
    if ($::Path::exists(base)) return {base.string()};
    return Collection(); // invalid collection
  }

  // prepare a pre-emptive path value here
  auto path = $::FS::Path(m_filter);

  // check if the baseline path immediately exists at all
  if ($::Path::is_directory(m_filter)) path = path / s_crate;
  if ($::Path::is_file(path)) return {path.string()};

  auto workspaces = Collection(); // prepare
  auto pattern = $::Glob::Pattern(m_filter);

  // iterate over the available pattern now
  for (const auto &entry : pattern.iterate({.directories = true})) {
    // prepare the incoming entry-path now
    path = $::FS::Path(entry.path);

    // attempt handling the incoming types now
    switch (entry.type) {
    default: continue; // ignore invalid ones
    case $::Glob::Archetype::REGULAR: break;
    case $::Glob::Archetype::DIRECTORY: path = path / s_crate; break;
    }

    // only append if the incoming path actually exists
    if ($::Path::exists(path)) workspaces.emplace_back(path.string());
  }

  // return the resulting workspaces now
  return workspaces;
}

$::Map::Dict<$::String::Buffer> Shell::Task::Branch::tasks(const Sabre::Crate::Manifest *crate) const {
  // ignore if the tasks if not given options
  if (crate == nullptr) return {};

  // prepare the output tasks to be used
  auto tasks = $::Map::Dict<$::String::Buffer>();

  // these are all the candidate keys available
  auto candidates = crate->tasks();

  // iterate over the incoming candidates now
  for (const auto &task : m_tasks) {
    // prepare the current filter to be used
    auto filter = $::Glob::Pattern(task);

    // check the current size of the tasks
    auto size = tasks.size();

    // check if we have any valid tasks to inherit
    for (const auto &[candidate, script] : candidates) {
      if (filter.matches(candidate)) tasks.emplace(candidate, script);
    }

    // if we found some candidates, then all good
    if (size != tasks.size() || !m_verbose) continue;

    // otherwise we declare a warning for each item not found
    std::cerr << $::Dye::yellow("Warning.Task").bold() << ": Named task '";
    std::cerr << $::Dye::bold(task) << "' could not be found\n";
  }

  // resolve the final output tasks
  return tasks;
}

//  PRIVATE METHODS  //

XPC::Process::Child Shell::Task::Branch::m_spawn($::String::Buffer script, const Sabre::Crate::Manifest *crate) const {
  // check for this executable if its available
  static constexpr $::String::View s_identifier = SABRE_MM_IDENTIFIER;

  // prepare some common paths to be used here
  static auto s_shell = XPC::Shell::Path::binary();
  static auto s_executable = $::Executable::resolve().string();

  // immediately fail if necessary
  $_EXPECT(s_shell.has_value(), "Could not find a suitable shell");

  // get the underlying working directory to be used
  auto cwd = $::FS::Path(crate->path());
  auto path = $::Environ::get("PATH").value_or("");
  auto initial = script.substr(0, script.find_first_of(' '));

  // update the current environment path with "node_modules/.bin"
  if (auto modules = cwd / "node_modules/.bin"; $::Path::exists(modules)) {
    path += XPC::Shell::Path::delimiter() + modules.string();
  }

  auto program = XPC::Shell::Path::resolve(initial, path); // find program
  if (program.has_value()) script = *program + script.substr(initial.size());

  /// TODO: modify the incoming script if necessary to do so (remove once installable in $PATH)
  if (script.starts_with(s_identifier)) script = s_executable + script.substr(s_identifier.size());

  // prepare the builder to be used now
  auto builder = XPC::Shell::Builder(*s_shell)
                     .cwd(cwd.string())
                     .envp("PATH", path)
                     .argv({$_PLATFORM_WINDOWS ? "/c" : "-c", script});

  // dispatch the resulting child instance now
  return XPC::Process::Child(builder);
}

bool Shell::Task::Branch::m_sequential(const Sabre::Crate::Manifest *crate) const {
  // iterate over the available scripts now
  for (const auto &[task, script] : tasks(crate)) {
    if (m_verbose) m_prettify(script); // show
    auto errc = m_spawn(script, crate).wait();
    if (errc == EXIT_SUCCESS) continue;
    return m_failure(task, errc), false;
  }

  // declare as a success
  return true;
}

bool Shell::Task::Branch::m_concurrent(const Sabre::Crate::Manifest *crate) const {
  // prepare the available tasks as well
  auto expanded = tasks(crate);

  // prepare the pending structure
  struct Pending {
    int32_t errc = 0;
    $::String::Buffer task;
    XPC::Process::Child child;
  };

  // prepare the output processes now
  auto processes = std::vector<Pending>();

  // pre-show all the tasks
  for (const auto &[_, script] : m_verbose ? expanded : $::Map::Dict<$::String::Buffer>()) m_prettify(script);

  // attempt spawning all the processes now
  for (const auto &[task, script] : expanded) {
    auto child = m_spawn(script, crate); // prepare the child
    Pending pending = {.task = task, .child = std::move(child)};
    processes.emplace_back(std::move(pending)); // and move
  }

  // wait for all the results to now occur and then show the results
  for (auto &pending : processes) pending.errc = pending.child.wait();
  for (const auto &pending : processes) m_failure(pending.task, pending.errc);

  // expecting all pending processes to have succeeded
  return std::ranges::all_of(processes, [](const Pending &pending) { return pending.errc == EXIT_SUCCESS; });
}

void Shell::Task::Branch::m_failure(const $::String::View &task, int32_t errc) const {
  // ignore if the result is a success
  if (errc == EXIT_SUCCESS) return;

  // otherwise we want to show the failure
  std::cerr << $::Dye::red("Exception.Task").bold() << ": Named task '";
  std::cerr << $::Dye::bold(task) << "' exited with code " << errc << std::endl;
}

void Shell::Task::Branch::m_prettify(const $::String::View &script) const noexcept {
  std::cout << $::Dye::dim("$ ").magenta() << $::Dye::dim(script) << std::endl;
}

void Shell::Task::Branch::m_print(std::ostream &os, const Branch &self) {
  os << '-' << (self.m_parallel ? 'p' : 's') << (self.m_verbose ? 'v' : 'q');
  if (self.m_filter.size()) os << " -w " << $::Path::relative(self.m_filter);
  os << fmt::format(" < {0}", fmt::join(self.m_tasks, " | ")); // emplace tasks
}
