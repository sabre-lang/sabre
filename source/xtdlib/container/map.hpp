#ifndef _XTDLIB_CONTAINER_MAP_HPP
#define _XTDLIB_CONTAINER_MAP_HPP

/// Vendor Includes
#include <ankerl/unordered_dense.h>

/// Library Includes
#include "xtdlib/string/buffer.hpp"
#include "xtdlib/string/view.hpp"

namespace $::Map {

/// @brief Prepare a suitable hasher to alias.
template <class T, class E = void> using Hash = ankerl::unordered_dense::hash<T, void>;

/// @brief Enables transparent hashing.
template <class T, class E = void> struct Transparent : public Hash<T, E> {
  using is_transparent = void;
};

/// @brief Baseline Map Typing.
template <class K, class T, class H = Hash<K>, class E = std::equal_to<K>>
using Base = ankerl::unordered_dense::map<K, T, H, E>;

/// @brief Explicit Set Typing (keys-only).
template <class K, class H = Hash<K>> using Set = ankerl::unordered_dense::set<K, H>;

/// @brief Explicit Dictionary Typing (buffer).
template <class T, class H = Transparent<String::View>> using Dict = Base<String::Buffer, T, H, std::equal_to<>>;

/// @brief Explicit Record Typing (view).
template <class T, class H = Hash<String::View>> using View = Base<String::View, T, H>;

} // namespace $::Map

#endif
