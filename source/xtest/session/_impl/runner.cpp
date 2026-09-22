/// Testing Includes
#include "xtest/session/runner.hpp"
#include "xtest/reporter/facade.hpp"

//  CONSTRUCTORS  //

XT::Session::Runner::Runner() : Runner($::Global::get<Options>()) {}
XT::Session::Runner::Runner(const Options *options) : m_options(options) {
  m_asserts = $::Unique::New<Assert::That>(this);
  m_reporter = $::Unique::New<Reporter::Facade>(this);
  m_statistics = $::Unique::New<Trivia::Statistics>();
  m_rng = $::Unique::New<Generator>(&m_options->ordering);
}
