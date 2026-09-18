#ifndef _SABRE_ATTRIBUTE_OVERLOAD_HPP
#define _SABRE_ATTRIBUTE_OVERLOAD_HPP

/// Syntax Includes
#include "sabre/syntax/annotation/signature.hpp"
#include "sabre/syntax/declaration/attribute.hpp"

namespace Sabre::Syntax {

/// @brief Compile Time Overload Node.
class Overload : public Mixin<Overload, Attribute> {
  //  PROPERTIES  //

  /// @brief The overload signature to bind.
  Signature *m_signature;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an overload attribute.
   * @param signature                 Signature to overload.
   */
  explicit Overload(Signature *signature) : Mixin(Reflect::Category::OVERLOAD), m_signature(signature) {}

  //  PUBLIC METHODS  //

  /// @brief The overload signature annotation.
  inline constexpr const Signature *signature() const noexcept { return m_signature; }
};

} // namespace Sabre::Syntax

#endif
