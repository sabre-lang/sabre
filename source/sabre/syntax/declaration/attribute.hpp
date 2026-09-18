#ifndef _SABRE_DECLARATION_ATTRIBUTE_HPP
#define _SABRE_DECLARATION_ATTRIBUTE_HPP

/// Sabre Include
#include "sabre/reflect/inspect.hpp"
#include "sabre/syntax/node.hpp"

namespace Sabre::Syntax {

/// @brief Compile Time Attribute Node.
class $_ABSTRACT Attribute : public Mixin<Attribute> {
  //  PROPERTIES  //

  /// @brief Associated compile-time category.
  Reflect::Category m_category;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a compile-time attribute.
   * @param category                  Reflection category.
   */
  explicit Attribute(Reflect::Category category) : m_category(category) {}

  //  PUBLIC METHODS  //

  /// @brief Assigned reflection category.
  inline constexpr Reflect::Category category() const noexcept { return m_category; }
};

} // namespace Sabre::Syntax

#endif
