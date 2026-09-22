#ifndef _XTEST_SESSION_RUNNER_HPP
#define _XTEST_SESSION_RUNNER_HPP

/// Testing Includes
#include "xtest/assert/that.hpp"
#include "xtest/reporter/abstract.hpp"
#include "xtest/session/options.hpp"
#include "xtest/trivia/statistics.hpp"

namespace XT::Session {

/// @brief Session Runner.
class Runner {
  //  PROPERTIES  //

  /// @brief The bound options to use.
  const Options *m_options = nullptr;

  /// @brief The random number generator to be used.
  $::Unique::Pointer<Generator> m_rng = nullptr;

  /// @brief The assertion interface.
  $::Unique::Pointer<Assert::That> m_asserts = nullptr;

  /// @brief The reporter to output to.
  $::Unique::Pointer<Reporter::Abstract> m_reporter = nullptr;

  /// @brief The statistics for this session runner.
  $::Unique::Pointer<Trivia::Statistics> m_statistics = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Handles constructing a session runner.
   * @param options             Options to inherit.
   */
  explicit Runner();
  explicit Runner(const Options *options);

  //  PUBLIC METHODS  //

  /// @brief Gets the associated options.
  inline constexpr const Options *options() const noexcept { return m_options; }

  /// @brief Gets the random number generator.
  inline constexpr Generator *rng() const noexcept { return m_rng.get(); }

  /// @brief Gets the assertion inteface.
  inline constexpr Assert::That *asserts() const noexcept { return m_asserts.get(); }

  /// @brief Gets the associated reporter.
  inline constexpr Reporter::Abstract *reporter() const noexcept { return m_reporter.get(); }

  /// @brief Gets the associated statistics.
  inline constexpr Trivia::Statistics *statistics() const noexcept { return m_statistics.get(); }
};

} // namespace XT::Session

#endif
