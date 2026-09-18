#ifndef _SABRE_RESOURCE_LOCATION_HPP
#define _SABRE_RESOURCE_LOCATION_HPP

/// Sabre Includes
#include "sabre/forward/resource.hpp"

namespace Sabre::Resource {

/// @brief View-Only Location Structure.
class Location {
  //  PROPERTIES  //

  /// @brief Range of location.
  XLSP::Range m_range = {};

  /// @brief Resource of location.
  $::URI::View m_resource = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an anonymous location.
  constexpr Location() = default;

  /**
   * @brief Constructs a base location.
   * @param resource              URI resource.
   * @param range                 Range to bind.
   */
  constexpr Location(const $::URI::View &resource, const XLSP::Range &range = {}) :
      m_range(range), m_resource(resource) {}

  //  OPERATOR METHODS  //

  /// @brief Equality operator for locations.
  inline constexpr auto operator==(const Location &other) const {
    return std::tie(m_range, m_resource) == std::tie(other.m_range, other.m_resource);
  }

  /// @brief Comparison operator for locations.
  inline constexpr auto operator<=>(const Location &other) const { return m_range <=> other.m_range; }

  //  PUBLIC METHODS  //

  /// @brief Gets the range of a resource location.
  inline constexpr XLSP::Range &range() noexcept { return m_range; }
  inline constexpr const XLSP::Range &range() const noexcept { return m_range; }

  /// @brief Gets the resource URI value.
  inline constexpr $::URI::View resource() const noexcept { return m_resource; }

  /// @brief Denotes if the location is anonymous.
  inline constexpr bool anonymous() const noexcept { return m_resource.anonymous(); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles formatting locations.
   * @param os                    Output stream.
   * @param self                  Location instance.
   */
  static inline void m_print(std::ostream &os, const Location &self) {
    os << self.m_resource.relative().string() << ':' << self.m_range.start;
  }
};

} // namespace Sabre::Resource

#endif
