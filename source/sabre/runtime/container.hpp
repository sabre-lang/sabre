#ifndef _SABRE_RUNTIME_CONTAINER_HPP
#define _SABRE_RUNTIME_CONTAINER_HPP

/// Sabre Includes
#include "sabre/forward/runtime.hpp"

namespace Sabre::Runtime {

/// @brief Runtime Services Container.
struct Container : public XI::Container {
  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted runtime container.
  explicit Container();

  /**
   * @brief Constructs a runtime container.
   * @param options                 Options to bind.
   */
  explicit Container(const Options &options);
};

} // namespace Sabre::Runtime

#endif
