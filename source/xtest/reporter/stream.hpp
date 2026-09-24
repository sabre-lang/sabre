#ifndef _XTEST_REPORTER_STREAM_HPP
#define _XTEST_REPORTER_STREAM_HPP

/// Testing Includes
#include "xtest/reporter/abstract.hpp"

namespace XT::Reporter {

/// @brief Reporter Stream Abstraction.
class $_ABSTRACT Stream : public Abstract {
  //  TYPEDEFS  //

  /// @brief Allow the facade internal access.
  friend class Facade;

protected:
  //  PROPERTIES  //

  /// @brief The current depth value.
  size_t m_depth = 0;

  /// @brief The underlying stream output.
  std::ostream &m_stream = std::cout;

public:
  //  CONSTRUCTORS  //

  /// @brief Only allow construction with a valid runner.
  constexpr Stream() = delete;

  /**
   * @brief Constructs a reporter.
   * @param runner            Session runner.
   * @param stream            Output stream.
   */
  constexpr Stream(Session::Runner *runner, std::ostream *stream = &std::cout) : Abstract(runner), m_stream(*stream) {}
};

} // namespace XT::Reporter

#endif
