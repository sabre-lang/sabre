/// Testing Includes
#include "xtest/bootstrap/launch.hpp"
#include "xtest/session/runner.hpp"

//  PUBLIC METHODS  //

int32_t XT::Bootstrap::launch(const Session::Storage *storage, const Session::Options &options) {
  return launch(storage->sections(), options);
}

int32_t XT::Bootstrap::launch(const $::Map::Dict<Handle::Group *> &sections, const Session::Options &options) {
  // prepare a runner instance to be used
  auto runner = Session::Runner(&options);

  // and start the session instance
  return launch(&runner, sections);
}

int32_t XT::Bootstrap::launch(Session::Runner *runner, const Session::Storage *storage) {
  return launch(runner, storage->sections());
}

int32_t XT::Bootstrap::launch(Session::Runner *runner, const $::Map::Dict<Handle::Group *> &sections) {
  // pull some common runner variables out
  auto total = sections.size();
  auto *reporter = runner->reporter();
  const auto *options = runner->options();

  // ensure that the reporter is actually valid
  $_ASSERT(reporter, "Unknown testing reporter '{0}'", options->reporter.label);

  // prepare the shuffled sections to be used now
  auto shuffled = runner->rng()->shuffle($::Ranges::To(sections | std::views::values), true);

  // iterate over the sections now to be execute with a count as well
  auto start = (reporter->before_running(total, options), $::Chrono::Point());
  for (const auto &section : shuffled) section->execute(runner);
  reporter->after_running(total, $::Chrono::Point() - start);

  // get the final success result to be returned
  return runner->statistics()->success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
