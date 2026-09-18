#ifndef _SABRE_TYPE_EXPORTS_HPP
#define _SABRE_TYPE_EXPORTS_HPP

/// Sabre Includes
#include "sabre/type/entity.hpp"

/// Type Includes
#include "sabre/type/compound/structure.hpp"

namespace Sabre::Type {

/// @brief Type Exports Environment.
class Exports {
  //  PROPERTIES  //

  /// @brief Underlying monotype value.
  $::Shared::Pointer<Structure> m_structure = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a set of exports.
   * @param fallback                Fallback typing.
   */
  explicit Exports(const Erased &fallback = nullptr) : Exports($::Shared::New<Structure>(fallback)) {}

  /**
   * @brief Constructs a set of exports.
   * @param structure               Structure to bind.
   */
  explicit Exports(const $::String::View &label) : Exports($::Shared::New<Structure>(label)) {}
  explicit Exports(const $::Shared::Pointer<Structure> &structure) : m_structure(structure) {}

  //  PUBLIC METHODS  //

  /// @brief Denotes if the exports are dynamic.
  inline constexpr bool dynamic() const noexcept { return monotype() != nullptr; }

  /// @brief Denotes if the exports are actually a failure.
  inline constexpr bool failure() const noexcept { return dynamic() && monotype()->is<Poison>(); }

  /// @brief Gets the structures internal fallback typing.
  inline constexpr const Erased &monotype() const noexcept { return m_structure->fallback(); }

  /// @brief Gets the core structure of exports.
  inline constexpr const $::Shared::Pointer<Structure> &structure() const noexcept { return m_structure; }
};

} // namespace Sabre::Type

#endif
