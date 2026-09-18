#ifndef _SABRE_BUNDLE_SERVICE_HPP
#define _SABRE_BUNDLE_SERVICE_HPP

/// Sabre Includes
#include "sabre/bundle/options.hpp"

namespace Sabre::Bundle {

/// @brief Core Runtime Service.
class Service : public XI::Singleton {
  //  PROPERTIES  //

  /// @brief Available services container.
  XI::Container *m_services;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a bundling service.
   * @param services                  Services container.
   */
  explicit Service();
  explicit Service(XI::Container *services);

  //  PUBLIC METHODS  //

  /**
   * @brief Handles bundling the runtime.
   * @param options                   Bundle options.
   */
  int32_t bundle(const Options &options);
};

} // namespace Sabre::Bundle

#endif
