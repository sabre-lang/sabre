#ifndef _SABRE_TYPE_ALGORITHM_HPP
#define _SABRE_TYPE_ALGORITHM_HPP

/// Sabre Includes
#include "sabre/type/entity.hpp"

namespace Sabre::Type {

/// @brief Encapsulates a lazy-algorithm for structure fields.
class Algorithm {
  //  TYPEDEFS  //

  /// @brief The underlying callback typing.
  using Callback = $::Shared::Functor<Entity(const Structure *) const>;

  //  PROPERTIES  //

  /// @brief The underlying container value.
  $::Map::Record<Callback> m_callbacks = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a baseline algorithm.
  constexpr Algorithm() = default;

  /**
   * @brief Handles emplacing fields.
   * @param fields                Fields to bind.
   */
  constexpr Algorithm(const $::Map::Record<Erased> &fields) {
    for (const auto &[name, field] : fields) emplace(name, field);
  }

  /**
   * @brief Handles emplacing fields.
   * @param fields                Fields to bind.
   */
  constexpr Algorithm(const $::Map::Record<Entity> &fields) {
    for (const auto &[name, field] : fields) emplace(name, field);
  }

  /**
   * @brief Constructs an algorithm from given callbacks.
   * @param callbacks             Callbacks to bind.
   */
  constexpr Algorithm(const $::Map::Record<Callback> &callbacks) : m_callbacks(callbacks) {}
  constexpr Algorithm($::Map::Record<Callback> &&callbacks) : m_callbacks(std::move(callbacks)) {}

  //  PUBLIC METHODS  //

  /**
   * @brief Checks if a field exists.
   * @param field                 Field to resolve.
   */
  inline constexpr bool contains(const $::String::View &field) const noexcept { return m_callbacks.contains(field); }

  /**
   * @brief Handles getting fields.
   * @param field                 Field to resolve.
   * @param structure             Structure instance.
   */
  inline constexpr Entity lookup(const $::String::View &field, const Structure *structure) const noexcept {
    return m_callbacks.contains(field) ? m_callbacks.at(field)(structure) : Entity();
  }

  /**
   * @brief Handles emplacing direct types.
   * @param name                  Name of field.
   * @param type                  Type of field.
   */
  inline constexpr bool emplace(const $::String::View &name, const Erased &type) { return emplace(name, Entity(type)); }

  /**
   * @brief Handles emplacing direct entities.
   * @param name                  Name of field.
   * @param entity                Field entity.
   */
  inline constexpr bool emplace(const $::String::View &name, const Entity &entity) {
    return m_callbacks.emplace(name, [entity](const Structure *) { return entity; }).second;
  }

  /**
   * @brief Handles emplacing direct callbacks.
   * @param name                  Name of field.
   * @param callback              Field callback.
   */
  inline constexpr bool emplace(const $::String::View &name, Callback &&callback) {
    return m_callbacks.emplace(name, std::move(callback)).second;
  }

  /**
   * @brief Gets a resolved view of entities.
   * @param structure             Structure instance.
   * @param constraints           Constraints to bind
   */
  inline constexpr auto view(const Structure *structure, Constraints *constraints = nullptr) const noexcept {
    // prepare the outgoing mapping of entities
    auto resolved = $::Map::Record<Entity>();

    // attempt binding each of our items now
    for (const auto &[name, callback] : m_callbacks) {
      auto entity = callback(structure); // get entity
      resolved.emplace(name, entity.infer(constraints));
    }

    // and return the resolved entities
    return resolved;
  }
};

} // namespace Sabre::Type

#endif
