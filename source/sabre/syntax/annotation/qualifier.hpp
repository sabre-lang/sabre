#ifndef _SABRE_ANNOTATION_QUALIFIER_HPP
#define _SABRE_ANNOTATION_QUALIFIER_HPP

/// Syntax Includes
#include "sabre/syntax/annotation/generics.hpp"
#include "sabre/syntax/literal/identifier.hpp"

namespace Sabre::Syntax {

/// @brief Qualifier Annotation.
class Qualifier : public Mixin<Qualifier, Annotation> {
  //  PROPERTIES  //

  /// @brief Generic template arguments.
  Specialization m_types = {};

  /// @brief Qualifier identifiers chain.
  std::vector<Identifier *> m_segments;

public:
  //  CONSTRUCTORS  //

  /// @brief Do not allow default construction.
  explicit Qualifier() = delete;

  /**
   * @brief Constructs a qualifier annotation.
   * @param segment                   Identifier segment.
   * @param types                     Typed arguments.
   */
  explicit Qualifier(Identifier *segment, const Specialization &types = {}) : m_types(types), m_segments({segment}) {}

  /**
   * @brief Constructs a qualifier annotation.
   * @param segments                  Identifier segments.
   * @param types                     Typed arguments.
   */
  explicit Qualifier(const std::vector<Identifier *> &segments, const Specialization &types = {})
      : m_types(types), m_segments(segments) {}

  //  PUBLIC METHODS  //

  inline constexpr const Specialization &types() const noexcept { return m_types; }
  inline constexpr const std::vector<Identifier *> &segments() const noexcept { return m_segments; }
};

} // namespace Sabre::Syntax

#endif
