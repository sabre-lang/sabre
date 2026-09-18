#ifndef _SABRE_BUILTINS_STORAGE_HPP
#define _SABRE_BUILTINS_STORAGE_HPP

/// Sabre Includes
#include "sabre/builtins/inspect.hpp"
#include "sabre/member/storage.hpp"

namespace Sabre::Builtins {

/**
 * @brief Allows constructing member storage.
 * @param initializer               Initializer to bind.
 */
template <class T>
static inline constexpr Member::Storage Storage(const Member::Storage::Initializer &initializer = {}) {
  return Member::Storage(Inspect<T>::name(), initializer);
}

} // namespace Sabre::Builtins

#endif
