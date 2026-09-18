#ifndef _SABRE_FUNCTION_INFO_HPP
#define _SABRE_FUNCTION_INFO_HPP

/// Sabre Includes
#include "sabre/bytecode/routine.hpp"
#include "sabre/forward/function.hpp"
#include "sabre/image/arena.hpp"
#include "sabre/resource/trace.hpp"

namespace Sabre::Function {

/// @brief Shared Function Information.
class Info {
  //  PROPERTIES  //

  /// @brief Internally linked code.
  Image::Slice m_bytecode = {};

  /// @brief Bound shared information.
  Bytecode::Shared m_shared = {};

  /// @brief Associated bytecode arena.
  Image::Arena *m_arena = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs shared function information.
   * @param arena                     Bytecode arena.
   * @param shared                    Shared information.
   */
  explicit Info(Image::Arena *arena = nullptr, const Bytecode::Shared &shared = {}) :
      m_shared(shared), m_arena(arena) {}

  //  PUBLIC METHODS  //

  /// @brief Minimum/maximum argument counts.
  inline constexpr size_t vargs() const noexcept { return m_shared.vargs; }
  inline constexpr size_t arity() const noexcept { return m_shared.arity; }
  inline constexpr size_t adicity() const noexcept { return m_shared.adicity; }
  inline constexpr size_t argmax() const noexcept { return std::min(m_shared.adicity, m_shared.vargs); }

  /// @brief Associated variable counts.
  inline constexpr size_t locals() const noexcept { return m_shared.locals; }
  inline constexpr size_t leaked() const noexcept { return m_shared.leaked; }

  /// @brief Denotes if this is the "main" module function information.
  inline constexpr bool main() const noexcept { return m_arena->functions.front().get() == this; }

  /// @brief Gets the underlying shared information.
  inline constexpr Bytecode::Shared *shared() noexcept { return &m_shared; }
  inline constexpr const Bytecode::Shared *shared() const noexcept { return &m_shared; }

  /// @brief Gets the underlying linkage view.
  inline constexpr Image::Slice &bytecode() noexcept { return m_bytecode; }
  inline constexpr const Image::Slice &bytecode() const noexcept { return m_bytecode; }

  /// @brief Associated arena instance.
  inline constexpr const Image::Arena *arena() const noexcept { return m_arena; }

  /// @brief Gets the bound resource value.
  inline constexpr $::URI::View resource() const noexcept { return m_arena->resource; }

  /// @brief Gets the associated resource group.
  inline constexpr auto group() const noexcept { return main() ? Resource::Group::SCRIPT : Resource::Group::METHOD; }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles printing shared function information.
   * @param os                    Output stream.
   * @param self                  Info to print.
   */
  static void m_print(std::ostream &os, const Info &self);
};

} // namespace Sabre::Function

#endif
