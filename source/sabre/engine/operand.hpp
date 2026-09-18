#ifndef _SABRE_ENGINE_OPERAND_HPP
#define _SABRE_ENGINE_OPERAND_HPP

/// Sabre Includes
#include "sabre/forward/bytecode.hpp"
#include "sabre/forward/engine.hpp"

namespace Sabre::Engine {

/// @brief Engine Operand Definition.
template <size_t N> struct $_ABSTRACT Operand {
  //  TYPEDEFS  //

  /// @brief Allow operands internal access.
  template <class...> friend struct Bytecode::Operands;

  //  CONSTRUCTORS  //

  /// @brief Default constructor.
  constexpr Operand() = default;

private:
  //  PRIVATE METHODS  //

  /// @brief Gets the underlying operand size.
  static inline constexpr uint64_t m_sizeof() noexcept { return N; }
};

} // namespace Sabre::Engine

#endif
