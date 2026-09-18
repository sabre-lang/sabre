#ifndef _SABRE_IMPORT_STORAGE_HPP
#define _SABRE_IMPORT_STORAGE_HPP

/// Sabre Includes
#include "sabre/module/abstract.hpp"
#include "sabre/value/symbol.hpp"

namespace Sabre::Import {

/// @brief Module Storage Container.
class Storage : public XI::Transient {
  //  TYPEDEFS  //

  /// @brief Storage Key Typing.
  using Key = Value::Symbol;

  /// @brief Storage Value Typing.
  using Value = $::Unique::Pointer<Module::Abstract>;

  //  PROPERTIES  //

  /// @brief Storage modification mutex.
  mutable $::Mutex::Auto m_mutex;

  /// @brief Bound module storage.
  $::Map::Base<Key, Value> m_modules = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a set of storage.
  explicit Storage() = default;

  //  PUBLIC METHODS  //

  /// @brief Removes all bound modules.
  inline constexpr void clear() { m_modules.clear(); }

  /// @brief Gets all the available storage for iteration.
  inline constexpr auto each() const noexcept { return m_modules | std::views::values; }

  /// @brief Denotes if the storage are empty.
  inline constexpr bool empty() const noexcept { return m_modules.empty(); }

  /// @brief Gets the total number of modules.
  inline constexpr size_t size() const noexcept { return m_modules.size(); }

  /**
   * @brief Checks if storage contains a resource.
   * @param resource              Resource value.
   */
  inline constexpr bool contains(Key resource) const { return m_modules.contains(resource); }

  /**
   * @brief Attempts getting a resource.
   * @param resource              Resource value.
   */
  inline constexpr Module::Abstract *lookup(Key resource) const {
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex); // pre-lock now
    auto iterator = m_modules.find(resource);  // attempt finding
    return iterator == m_modules.end() ? nullptr : iterator->second.get();
  }

  /**
   * @brief Handles emplacing modules.
   * @param module                Module to emplace.
   */
  inline constexpr Module::Abstract *emplace(Value &&module) {
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex);
    auto symbol = Key(module->resource());
    if (contains(symbol)) return m_modules.at(symbol).get();
    return m_modules.emplace(symbol, std::move(module)).first->second.get();
  }

  /**
   * @brief Handles removing modules.
   * @param resource              Resource value.
   */
  inline constexpr void remove(Key resource) {
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex);
    m_modules.erase(resource); // and remove
  }
};

} // namespace Sabre::Import

#endif
