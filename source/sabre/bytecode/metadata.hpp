#ifndef _SABRE_BYTECODE_METADATA_HPP
#define _SABRE_BYTECODE_METADATA_HPP

/// Sabre Modules
#include "sabre/function/info.hpp"
#include "sabre/module/metadata.hpp"
#include "sabre/runtime/isolate.hpp"

namespace Sabre::Module {

/// @brief Constructs a set of bytecode metadata.
template <> class Metadata::Wrapper<Phase::COMPILED> : public Metadata {
  //  PROPERTIES  //

  /// @brief Associated output arena.
  $::Unique::Pointer<Image::Arena> m_arena = $::Unique::New<Image::Arena>();

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a set of compilation exports.
  explicit Wrapper() = default;

  //  PUBLIC METHODS  //

  /// @brief Image compilation outputs.
  inline constexpr $::Unique::Pointer<Image::Arena> &arena() noexcept { return m_arena; }
  inline constexpr const Image::Arena *arena() const noexcept { return m_arena.get(); }
};

} // namespace Sabre::Module

#endif
