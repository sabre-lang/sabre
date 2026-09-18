#ifndef _SABRE_ANNOTATION_INTERFACE_HPP
#define _SABRE_ANNOTATION_INTERFACE_HPP

/// Syntax Includes
#include "sabre/syntax/declaration/variable.hpp"

namespace Sabre::Syntax {

/// @brief Interface Annotation Node.
class Interface : public Mixin<Interface, Annotation> {
  //  PROPERTIES  //

  /// @brief Record elements.
  std::vector<Syntax::Variable *> m_properties = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an interface annotation.
   * @param properties                Interface fields.
   */
  explicit Interface(const std::vector<Syntax::Variable *> &properties = {}) : m_properties(properties) {}

  //  PUBLIC METHODS  //

  inline constexpr bool empty() const noexcept { return m_properties.empty(); }
  inline constexpr size_t size() const noexcept { return m_properties.size(); }
  inline constexpr const std::vector<Syntax::Variable *> &properties() const noexcept { return m_properties; }
};

} // namespace Sabre::Syntax

#endif
