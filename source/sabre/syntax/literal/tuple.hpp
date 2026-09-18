#ifndef _SABRE_LITERAL_TUPLE_HPP
#define _SABRE_LITERAL_TUPLE_HPP

/// Sabre Includes
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Tuple Literal Node.
class Tuple : public Mixin<Tuple, Expression> {
  //  PROPERTIES  //

  /// @brief List elements.
  std::vector<Expression *> m_elements = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a list literal.
   * @param elements                  Tuple elements.
   */
  explicit Tuple(const std::vector<Expression *> &elements = {}) : m_elements(elements) {}

  //  PUBLIC METHODS  //

  inline constexpr bool empty() const noexcept { return m_elements.empty(); }
  inline constexpr size_t size() const noexcept { return m_elements.size(); }
  inline constexpr const std::vector<Expression *> &elements() const noexcept { return m_elements; }
};

} // namespace Sabre::Syntax

#endif
