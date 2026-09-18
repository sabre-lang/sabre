#ifndef _SABRE_ASYNC_ENTRY_HPP
#define _SABRE_ASYNC_ENTRY_HPP

/// Sabre Includes
#include "sabre/async/service.hpp"
#include "sabre/runtime/executor.hpp"

namespace Sabre::Async {

/// @brief Describes a main-entry isolate.
struct $_ABSTRACT Entry : public Runtime::Executor {
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a main-isolate.
   * @param args              Executor arguments.
   */
  template <class... As> explicit Entry(As &&...args) : Executor(std::forward<As>(args)...) {
    service<Service>()->m_isolate = this;
  }
};

} // namespace Sabre::Async

#endif
