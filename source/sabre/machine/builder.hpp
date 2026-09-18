#ifndef _SABRE_MACHINE_BUILDER_HPP
#define _SABRE_MACHINE_BUILDER_HPP

/// Sabre Modules
#include "sabre/machine/allocator.hpp"
#include "sabre/machine/emitter.hpp"
#include "sabre/machine/facts.hpp"
#include "sabre/machine/logger.hpp"

namespace Sabre::Machine {

/// @brief Scoped Machine-Code Builder.
struct Builder : public XI::Transient {
  //  PROPERTIES  //

  Register::Host envp = {};     // Context register.
  Register::Host argv = {};     // Arguments registers.
  Register::Host params = {};   // Parameters registers.
  Register::Host isolate = {};  // Isolate register.
  Register::Host frame = {};    // Call-frame register.
  Register::Host feedback = {}; // Feedback register.
  Register::Host result = {};   // Accumulator register.

  Label panic = {};     // Enables panic handling.
  Label interrupt = {}; // Enables interrupt handling.

  /// @brief Optimization facts.
  Facts facts = {};

  /// @brief The available labels.
  std::vector<Label> labels = {};

  /// @brief Incoming function information.
  Info *info = nullptr;

  /// @brief The underlying scoped compiler.
  Compiler *compiler = nullptr;

  /// @brief Handles logging headers and comments.
  $::Unique::Pointer<Logger> logger = $::Unique::New<Logger>(compiler);

  /// @brief Handles emitting complex instrutions.
  $::Unique::Pointer<Emitter> emitter = $::Unique::New<Emitter>(this);

  /// @brief Handles emitting register allocations.
  $::Unique::Pointer<Allocator> registers = $::Unique::New<Allocator>(compiler);

  //  CONSTRUCTORS  //

  /// @brief Do not allow default construction.
  constexpr Builder() = delete;

  /// @brief Constructs a defaulted builder.
  constexpr Builder(Info *info, Compiler *compiler) : facts(info), info(info), compiler(compiler) { m_prolog(); }

  /// @brief Handles closing off the builder.
  constexpr ~Builder() { m_epilog(); }

private:
  //  PRIVATE METHODS  //

  /// @brief Construct the builders prolog.
  void m_prolog();

  /// @brief Constructs the builders epilog.
  void m_epilog();
};

} // namespace Sabre::Machine

#endif
