#ifndef _SABRE_RELINT_SERVICE_HPP
#define _SABRE_RELINT_SERVICE_HPP

/// Sabre Includes
#include "sabre/crate/registry.hpp"
#include "sabre/forward/value.hpp"
#include "sabre/relint/hooks.hpp"

namespace Sabre::Relint {

/// @brief The linter service contains all available rules.
class Service : public XI::Singleton {
  //  PROPERTIES  //

  /// @brief Available services container.
  XI::Container *m_services = nullptr;

  /// @brief The default options available.
  const Options *m_options = nullptr;

  /// @brief All available subscribed rules.
  $::Map::Record<$::Unique::Pointer<Rule>> m_rules = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Default constructor.
  explicit Service();
  explicit Service(XI::Container *services);

  //  PUBLIC METHODS  //

  /**
   * @brief Handles subscribing rules.
   * @param rule                      Rule to subscribe.
   */
  inline constexpr bool subscribe($::Unique::Pointer<Rule> &&rule) {
    return m_rules.emplace(rule->documentation.identifier, std::move(rule)).second;
  }

  /**
   * @brief Handles subscribing rules.
   * @param identifier                Rule identifier.
   * @param rule                      Rule to subscribe.
   */
  inline constexpr bool subscribe(const $::String::Buffer &identifier, $::Unique::Pointer<Rule> &&rule) {
    return rule->documentation.identifier = identifier, subscribe(std::move(rule));
  }

  /**
   * @brief Handles resolving delegate hooks.
   * @param args                      Crate scanning arguments.
   */
  template <class... As> inline constexpr $::Unique::Pointer<Hooks> hooks(As &&...args) {
    return m_hooks(Crate::Registry::scan(std::forward<As>(args)...)->lint());
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles emplacing plugins.
   * @param options                   Options to update with plugins.
   */
  void m_plugins(Options &options) noexcept;

  /**
   * @brief Handles merging options with global ones.
   * @param options                   Options to merge with globals.
   */
  void m_merge(Options &options) const noexcept;

  /**
   * @brief Handles setting all the recommended options.
   * @param options                   Options to set recommendations.
   */
  void m_recommends(Options &options) const noexcept;

  /**
   * @brief Handles merging, plugins and setting recommended settings.
   * @param options                   Options to merge.
   */
  Options &m_resolve(Options &options) noexcept;

  /**
   * @brief Handles subscribing plugins.
   * @param module                    Module to subscribe.
   */
  void m_subscribe(const Value::Any &module, Options &options) noexcept;

  /**
   * @brief Constructs hooks from the incoming options.
   * @param options                   Options to use.
   */
  $::Unique::Pointer<Hooks> m_hooks(Options options) noexcept;
};

} // namespace Sabre::Relint

#endif
