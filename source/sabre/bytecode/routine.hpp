#ifndef _SABRE_BYTECODE_ROUTINE_HPP
#define _SABRE_BYTECODE_ROUTINE_HPP

/// Sabre Includes
#include "sabre/bytecode/block.hpp"
#include "sabre/string/intern.hpp"
#include "sabre/value/common.hpp"

/// Syntax Includes
#include "sabre/syntax/literal/lambda.hpp"

namespace Sabre::Bytecode {

/// @brief Shared Bytecode Structure.
struct Shared {
  //  PROPERTIES  //

  uint32_t locals = 0; // Total locals count allowed.
  uint32_t leaked = 0; // Total leaked variables allowed.

  uint64_t arity = 0;   // Expected minimum arguments.
  uint64_t adicity = 0; // Expected maximum arguments.

  /// @brief The expected variadic arguments index to be used.
  uint64_t vargs = UINT64_MAX;

  //  CONSTUCTORS  //

  /// @brief Constructs a set of defaulted details.
  constexpr Shared() = default;
};

/// @brief Callable Bytecode Sections.
struct Routine {
  //  PROPERTIES  //

  /// @brief The top-most label.
  Label head = 0;

  /// @brief The underlying shared information.
  Shared *shared = nullptr;

  /// @brief Currently available blocks.
  std::vector<$::Unique::Pointer<Block>> blocks = {};

  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a routine instance.
   * @param shared                  Shared bytecode information.
   */
  explicit Routine(Shared *shared = nullptr) : shared(shared) { $_ASSERT(shared != nullptr); }
};

} // namespace Sabre::Bytecode

#endif
