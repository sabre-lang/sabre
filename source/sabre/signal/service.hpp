#ifndef _SABRE_SIGNAL_SERVICE_HPP
#define _SABRE_SIGNAL_SERVICE_HPP

/// Sabre Includes
#include "sabre/async/service.hpp"
#include "sabre/forward/signal.hpp"

namespace Sabre::Signal {

/// @brief Signal Handling Service.
class Service : public XI::Singleton, public XI::Immediate {
  //  PROPERTIES  //

  /// @brief Associated asynchronous runtime.
  Async::Service *m_async = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a signal service.
   * @param services                  Services container.
   */
  explicit Service();
  explicit Service(XI::Container *services);

  //  PUBLIC METHODS  //

  /**
   * @brief Handles raising signals.
   * @param code                      Signal code/name.
   */
  inline constexpr void raise(Symbolic code) { m_async->signals()->raise(code); }
  inline constexpr void raise(const $::String::View &name) { m_async->signals()->raise(name); }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles delegating incoming signals.
   * @param code                      Signal code.
   */
  void m_dispatch(const Code &code);
};

} // namespace Sabre::Signal

#endif
