#ifndef _SABRE_RESOURCE_TRACE_HPP
#define _SABRE_RESOURCE_TRACE_HPP

/// Sabre Includes
#include "sabre/forward/resource.hpp"

//  X-MACROS  //

#define XX_RESOURCE_GROUPS(X) \
  X(SCRIPT, "Script", "in")   \
  X(FUTURE, "Future", "in")   \
  X(NATIVE, "Native", "in")   \
  X(METHOD, "Function", "in") \
  X(IMPORT, "Import", "from")

//  NAMESPACES  //

namespace Sabre::Resource {

/// @brief Available Resource Groups.
$_XX_ENUM_CLASS(Group, uint8_t, XX_RESOURCE_GROUPS);

/// @brief Resource Trace Details.
class Trace {
  //  PROPERTIES  //

  /// @brief Encapsulated group.
  Group m_group;

  /// @brief Associated resource view.
  $::URI::View m_resource;

  /// @brief The reference location.
  const XLSP::Position *m_position;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a resource trace.
   * @param resource                  Document resource.
   * @param group                     Group to be bound.
   * @param position                  Trace position.
   */
  constexpr Trace(const $::URI::View &resource = {}, const XLSP::Position *position = nullptr) :
      Trace(resource, Group::METHOD, position) {}

  /**
   * @brief Constructs a resource trace.
   * @param resource                  Document resource.
   * @param group                     Group to be bound.
   * @param position                  Trace position.
   */
  constexpr Trace(const $::URI::View &resource, Group group, const XLSP::Position *position = nullptr) :
      m_group(group), m_resource(resource), m_position(position) {}

  //  PUBLIC METHODS  //

  inline constexpr Group group() const noexcept { return m_group; }
  inline constexpr $::URI::View resource() const noexcept { return m_resource; }
  inline constexpr const XLSP::Position *position() const noexcept { return m_position; }

  /// @brief Checks if a resource-trace is anonymous.
  inline constexpr bool anonymous() const noexcept { return m_position == nullptr && m_resource.anonymous(); }

protected:
  //  PRIVATE METHODS  //

  /// @brief Handles describing traces.
  inline constexpr $::String::View m_describe() const noexcept {
#define X(G, N, D, ...)          \
  case Group::G: return D " " N;
    switch (m_group) { XX_RESOURCE_GROUPS(X) default : $_ABORT("Unexpected trace-group"); }
#undef X
  };

  /**
   * @brief Handles formatting traces.
   * @param os                        Output stream.
   * @param self                      Trace instance.
   */
  static inline void m_print(std::ostream &os, const Trace &self) {
    // show the incoming resource instance now
    os << self.m_describe() << " at ";
    os << self.resource().relative().string();

    // stop if the position is invalid at all
    if (self.m_position == nullptr) return;

    // format the position when it is given
    os << fmt::format(":{0}:{1}", self.m_position->line, self.m_position->column);
  }
};

} // namespace Sabre::Resource

//  UNDEFINES  //

#undef XX_RESOURCE_GROUPS

#endif
