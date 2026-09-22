/// Testing Includes
#include "xtest/handle/group.hpp"
#include "xtest/assert/reason.hpp"
#include "xtest/session/runner.hpp"

//  PUBLIC METHODS  //

size_t XT::Handle::Group::m_count() const noexcept { return m_tests.size(); }

void XT::Handle::Group::m_execute(Session::Runner *runner) const {
  // destructure some common items now
  auto *reporter = runner->reporter();

  // begin the group section
  reporter->group_opened(this);

  // determine if skipping this group at all
  auto skip = !m_initialize(runner) || trivia()->skip || trivia()->todo;

  // shuffle the tests to potentially be run
  auto shuffled = skip ? std::vector<const Base *>() : runner->rng()->shuffle(m_tests);

  // allow testing if the group is not skipped or unimplemented
  for (const auto *test : shuffled) test->execute(runner);

  // close the group section
  reporter->group_closed(this);
}

//  PRIVATE METHODS  //

bool XT::Handle::Group::m_initialize(Session::Runner *runner) const {
  // stop immediately if valid at all
  if (m_setup == nullptr) return true;

  // clang-format off
  try { return m_setup(runner), true; } // validly setup the group
  catch (...) { return runner->reporter()->group_failure(this, Assert::Reason()), false; }
  // clang-format on
}
