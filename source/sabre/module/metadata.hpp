#ifndef _SABRE_MODULE_METADATA_HPP
#define _SABRE_MODULE_METADATA_HPP

/// Sabre Includes
#include "sabre/module/phase.hpp"

namespace Sabre::Module {

/// @brief Metadata Abstraction.
struct $_ABSTRACT Metadata {
  //  TYPEDEFS  //

  /// @brief Available metadata attributes.
  template <Phase P> class Wrapper;

  //  CONSTRUCTORS  //

  /// @brief Virtual abstract destructor.
  virtual ~Metadata() = default;
};

} // namespace Sabre::Module

#endif
