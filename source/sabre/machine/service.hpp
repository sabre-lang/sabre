#ifndef _SABRE_MACHINE_SERVICE_HPP
#define _SABRE_MACHINE_SERVICE_HPP

/// Sabre Includes
#include "sabre/machine/info.hpp"

namespace Sabre::Machine {

/// @brief Machine Code Service.
class Service : public XI::Singleton {
  //  PROPERTIES  //

  /// @brief Available services container.
  XI::Container *m_services = nullptr;

  /// @brief Internal compilation context.
  XASM::Runtime::Context m_context = {};

  /// @brief All compiled functions.
  $::Map::Set<$::Shared::Pointer<Info>> m_callees = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a machine-code service.
  explicit Service();
  explicit Service(XI::Container *services);

  //  PUBLIC METHODS  //

  /**
   * @brief Validates a closure can be compiled to machine-code.
   * @param closure               Closure function.
   */
  bool verify(const Function::Info *info) const noexcept;

  /**
   * @brief Attempts lifting a closure to machine-code.
   * @param isolate               Runtime isolate.
   * @param closure               Closure to upgrade.
   */
  Function::Any upgrade(Runtime::Isolate *isolate, const Function::Closure &closure);

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles dumping the current logging outputs.
   * @param info                  Function information.
   */
  void m_dump(const Function::Info *info) const noexcept;

  /**
   * @brief Handles internal compilation.
   * @param callee                Callee information.
   * @param compiler              Universal compiler.
   */
  void m_compile(Info *callee, Compiler *compiler) const noexcept;
};

} // namespace Sabre::Machine

#endif
