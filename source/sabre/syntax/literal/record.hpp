#ifndef _SABRE_LITERAL_RECORD_HPP
#define _SABRE_LITERAL_RECORD_HPP

/// Syntax Includes
#include "sabre/syntax/declaration/variable.hpp"

namespace Sabre::Syntax {

/// @brief Record Literal Node.
class Record : public Mixin<Record, Expression> {
  //  PROPERTIES  //

  /// @brief Record elements.
  std::vector<Variable *> m_elements = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a list literal.
   * @param elements                Record elements.
   */
  explicit Record(const std::vector<Variable *> &elements = {}) : m_elements(elements) {}

  //  PUBLIC METHODS  //

  inline constexpr bool empty() const noexcept { return m_elements.empty(); }
  inline constexpr size_t size() const noexcept { return m_elements.size(); }
  inline constexpr const std::vector<Variable *> &elements() const noexcept { return m_elements; }
};

} // namespace Sabre::Syntax

#endif
