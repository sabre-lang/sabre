#ifndef _SABRE_TESTING_SERVICE_HPP
#define _SABRE_TESTING_SERVICE_HPP

/// Sabre Includes
#include "sabre/runtime/options.hpp"
#include "sabre/testing/storage.hpp"

namespace Sabre::Testing {

/// @brief Constructs a testing service.
class Service : public XI::Singleton {
  //  PROPERTIES  //

  /// @brief Available services container.
  XI::Container *m_services;

  /// @brief The current session instance.
  Runner *m_session = nullptr;

  /// @brief The underlying registry instance.
  $::Unique::Pointer<Storage> m_storage = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a runtime service.
   * @param services                  Services container.
   */
  explicit Service();
  explicit Service(XI::Container *services);

  //  PUBLIC METHODS  //

  /// @brief Gets the current testing session.
  inline constexpr Runner *session() noexcept { return m_session; }
  inline constexpr const Runner *session() const noexcept { return m_session; }

  /// @brief Gets the testing registry.
  inline constexpr Storage *storage() noexcept { return m_storage.get(); }
  inline constexpr const Storage *storage() const noexcept { return m_storage.get(); }

  /// @brief Spawns the current testing runtime.
  inline constexpr int32_t spawn() { return m_spawn(); }

  /**
   * @brief Requests a testing session.
   * @param options             Options to use.
   */
  inline constexpr int32_t invoke(const Options &options = {}) { return m_invoke(options); }

private:
  //  PRIVATE METHODS  //

  /// @brief Handles launching the testing runtime.
  int32_t m_spawn();

  /**
   * @brief Requests a testing session.
   * @param options             Options to use.
   */
  int32_t m_invoke(const Options &options);
};

} // namespace Sabre::Testing

#endif
