#ifndef _SABRE_TESTING_STORAGE_HPP
#define _SABRE_TESTING_STORAGE_HPP

/// Sabre Includes
#include "sabre/testing/options.hpp"

namespace Sabre::Testing {

/// @brief Testing Storage.
struct Storage : public XI::Transient, XT::Session::Storage {
  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted set of storage.
  constexpr Storage() = default;
};

} // namespace Sabre::Testing

#endif
