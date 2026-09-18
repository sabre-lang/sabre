#ifndef _SABRE_RELINT_RULE_HPP
#define _SABRE_RELINT_RULE_HPP

/// Sabre Includes
#include "sabre/forward/relint.hpp"

namespace Sabre::Relint {

/// @brief Rule Documentation Details.
struct Documentation {
  //  PROPERTIES  //

  /// @brief An associated resource buffer.
  $::URI::View resource = {};

  /// @brief The associated rule identifier (no need to assign).
  $::String::Buffer identifier = "sabre/unknown";

  /// @brief The baseline message format for a rule.
  $::String::Buffer message = "Rule error message...";

  /// @brief The documentation description for a rule.
  $::String::Buffer description = "Rule description...";
};

/// @brief Relint Rule Definition.
struct Rule {
  //  PROPERTIES  //

  /// @brief Denotes if the rule is deprecated.
  bool deprecated = false;

  /// @brief Denotes if a rule is recommended.
  bool recommended = false;

  /// @brief Defaulted severity type.
  Severity severity = Severity::WARN;

  /// @brief Handles constructing rules.
  Factory factory = [](Hooks *, const Rule *) {};

  /// @brief Rule documentation structure.
  Documentation documentation = {};
};

/// @brief Static Rule Registry.
template <$::String::Literal N> struct Register : public $::Ensure::Static {
  //  PUBLIC METHODS  //

  /// @brief Gets the available registration name.
  static inline constexpr $::String::View label() noexcept { return N.view(); }

  /// @brief Handles defining the base rule.
  static Rule define() noexcept;

  /**
   * @brief Handles registration of the rule.
   * @param hooks                   Hooks service.
   * @param rule                    Rule subscribed.
   */
  static void subscribe(Hooks *hooks, const Rule *rule) noexcept;
};

} // namespace Sabre::Relint

#endif
