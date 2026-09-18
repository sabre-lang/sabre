#ifndef _SABRE_TYPE_WORLD_HPP
#define _SABRE_TYPE_WORLD_HPP

/// Sabre Includes
#include "sabre/flow/control.hpp"
#include "sabre/type/entity.hpp"

/// Syntax Includes
#include "sabre/syntax/declaration/preamble.hpp"

namespace Sabre::Type {

/// @brief Type World Container.
class World {
  //  TYPEDEFS  //

  /// @brief Allow the analyzer internal access.
  friend class Analyzer;

  /// @brief Allows Viewing Slices of Types.
  template <auto E, auto A> class Slice {
    //  PROPERTIES  //

    /// @brief Encapsulated world instance.
    World *m_world;

  public:
    //  CONSTRUCTORS  //

    /// @brief Do not allow default construction.
    constexpr Slice(...) = delete;

    /**
     * @brief Handles construcing slices.
     * @param world                 World to slice.
     */
    constexpr Slice(World *world) : m_world(world) {}

    //  PUBLIC METHODS  //

    /**
     * @brief Checks if a world contains an entity.
     * @param name                  Entity name.
     * @param local                 Locality check.
     */
    inline constexpr bool contains(const $::String::View &name, bool local = true) const {
      const auto *entity = m_world->lookup(name, local);
      return entity && E(*entity); // check existence now
    }

    /**
     * @brief Attempts declaring a named entity.
     * @param name                  Entity name.
     * @param type                  Type to bind.
     * @param range                 Optional range.
     */
    inline constexpr Entity *declare(const $::String::View &name, const Erased &type, const XLSP::Range &range = {}) {
      auto &entity = m_emplace(name, range);
      if (E(entity)) return nullptr;
      return A(entity, type), &entity;
    }

    /**
     * @brief Handles declaring entities from syntax.
     * @param declaration           Variable declaration.
     * @param type                  Typing to be bound.
     */
    template <std::derived_from<Syntax::Declaration> T>
    inline constexpr Entity *declare(const T *declaration, const Erased &type) {
      // use the baseline declaration handler to define the type
      auto *entity = declare(declaration->name(), type, declaration->trivia()->bounds());

      // "normally" should not have errors with correct code
      if ($_UNLIKELY(entity == nullptr)) return nullptr;

      // update some qualities about the entity now
      entity->modifiers() |= declaration->modifiers();
      if (entity->exported()) entity->unused(false);

      // and return the resulting entity
      return entity;
    }

  private:
    //  PRIVATE METHODS  //

    /**
     * @brief Handles emplacing entities.
     * @param name                  Entity name.
     * @param range                 Optional range.
     */
    inline constexpr Entity &m_emplace(const $::String::View &name, const XLSP::Range &range = {}) {
      auto &entity = m_world->m_entities.try_emplace(name).first->second;
      return m_world->m_ranges.try_emplace(name, range), entity;
    }
  };

  /// @brief All available getters.
  struct Getter : public $::Ensure::Static {
    //  TYPEDEFS  //

    /// @brief Sets a depth property.
    template <Flow::Degree World::*D>
    static constexpr auto extent = [](this const auto &self, const auto *world) -> Flow::Degree {
      if (world == nullptr) return -1;
      if ((*world).*D > -1) return (*world).*D;
      $_MUSTTAIL return self(world->m_parent);
    };

    /// @brief Callee getter.
    static constexpr auto callee = [](this const auto &self, const auto *world) -> Erased {
      if (world == nullptr) return nullptr;
      if (world->m_callee) return world->m_callee;
      return self(world->m_parent); // recursive
    };
  };

  /// @brief All available setters.
  struct Setter : public $::Ensure::Static {
    //  PROPERTIES  //

    /// @brief Sets a depth property.
    template <Flow::Degree World::*D>
    static constexpr auto extent = [](auto *world, const Flow::Degree &value) { (*world).*D = value; };

    /// @brief Callee setter.
    static constexpr auto callee = [](auto *world, const Erased &value) {
      world->m_callee = value; // update the current callee
      world->m_loops = -1, world->m_outer = world->m_depth;
    };
  };

  /// @brief Callee Accessor Typing.
  using Callee = $::Property::Accessor<World, Getter::callee, Setter::callee>;

  /// @brief Flow Accessor Typing.
  template <Flow::Degree World::*D> using Extent = $::Property::Accessor<World, Getter::extent<D>, Setter::extent<D>>;

  //  PROPERTIES  //

  Flow::Degree m_depth = -1; // Current world depth.
  Flow::Degree m_outer = -1; // Outer context depth.
  Flow::Degree m_loops = -1; // Current loop depth.

  /// @brief The explicit callee value.
  Erased m_callee = nullptr;

  /// @brief The parent world instance.
  World *m_parent = nullptr;

  /// @brief Current preamble target.
  Entity *m_preamble = nullptr;

  /// @brief The bound analyzer instance.
  Analyzer *m_analyzer = nullptr;

  /// @brief Encapsulated entities.
  $::Map::Record<Entity> m_entities = {};

  /// @brief Currently deferred values.
  std::vector<Deferrer> m_deferred = {};

  /// @brief Resource locations for cleanup errors.
  $::Map::Record<XLSP::Range> m_ranges = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a global world instance.
  explicit World() = default;

  /**
   * @brief Constructs a world instance with an analyzer.
   * @param analzyer                Analyzer to bind.
   */
  explicit World(Analyzer *analyzer);

  /**
   * @brief Constructs a world instance with a parent.
   * @param parent                  Parent world.
   * @param analyzer                Analyzer to bind.
   */
  explicit World(World *parent, Analyzer *analzyer = nullptr);

  /// @brief Handles destructing the world instance.
  ~World();

  //  PUBLIC METHODS  //

  /// @brief Gets the current world depth.
  inline constexpr Flow::Degree depth() const noexcept { return m_depth; }

  /// @brief Gets the current preamble target.
  inline constexpr Entity *preamble() const noexcept { return m_preamble; }

  /// @brief Gets the import/export state.
  inline constexpr bool importable() const noexcept { return m_depth < 1; }
  inline constexpr bool exportable() const noexcept { return m_callee == nullptr; }

  /// @brief Gets the underlying entities map.
  inline constexpr $::Map::Record<Entity> &entities() noexcept { return m_entities; }
  inline constexpr const $::Map::Record<Entity> &entities() const noexcept { return m_entities; }

  /// @brief Gets/sets the current callee value.
  inline constexpr Callee callee() noexcept { return Callee(this); }
  inline constexpr Erased callee() const noexcept { return Getter::callee(this); }

  /// @brief Gets/sets the current looping depth.
  inline constexpr auto loops() noexcept { return Extent<&World::m_loops>(this); }
  inline constexpr Flow::Degree loops() const noexcept { return Getter::extent<&World::m_loops>(this); }

  /// @brief Gets/sets the current outer depth.
  inline constexpr auto outer() noexcept { return Extent<&World::m_outer>(this); }
  inline constexpr Flow::Degree outer() const noexcept { return Getter::extent<&World::m_outer>(this); }

  /// @brief Gets the world-of-types view.
  inline constexpr auto types() {
    static constexpr auto s_exists = [](const Entity &entity) { return entity.transient(); };
    static constexpr auto s_assign = [](Entity &entity, const Erased &type) { entity.type() = type; };
    return Slice<s_exists, s_assign>(this); // construct the outgoing view-slice to be used now
  }

  /// @brief Gets the world-of-values view.
  inline constexpr auto values() {
    static constexpr auto s_exists = [](const Entity &entity) { return entity.opaque(); };
    static constexpr auto s_assign = [](Entity &entity, const Erased &type) { entity.value() = type; };
    return Slice<s_exists, s_assign>(this); // construct the outgoing view-slice to be used now
  }

  /**
   * @brief Handles looking up internal values.
   * @param name                    Name of entity.
   * @param local                   Locality check.
   */
  inline constexpr Entity *lookup(const $::String::View &name, bool local = false) const {
    auto iter = m_entities.find(name); // scan here first to get details
    if (iter != m_entities.cend()) return const_cast<Entity *>(&iter->second);
    return m_parent && !local ? m_parent->lookup(name) : nullptr; // search more
  }

  /**
   * @brief Declares an empty entity.
   * @param name                    Name of entity.
   * @param range                   Optional range.
   */
  inline constexpr bool declare(const $::String::View &name, const XLSP::Range &range = {}) {
    return declare(name, {}, range);
  }

  /**
   * @brief Declares a named entity.
   * @param name                    Name of entity.
   * @param entity                  Entity to bind.
   * @param range                   Optional range.
   */
  inline constexpr bool declare(const $::String::View &name, const Entity &entity, const XLSP::Range &range = {}) {
    if (!m_entities.try_emplace(name, entity).second) return false;
    return m_ranges.try_emplace(name, range), true; // valid entity
  }
};

} // namespace Sabre::Type

#endif
