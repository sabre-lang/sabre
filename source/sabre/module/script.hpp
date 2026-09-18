#ifndef _SABRE_MODULE_SCRIPT_HPP
#define _SABRE_MODULE_SCRIPT_HPP

/// Sabre Includes
#include "sabre/module/abstract.hpp"

namespace Sabre::Module {

/// @brief Script Module Implementation.
struct Script : public Mixin<Script> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles parsing a module.
   * @param services                  Service container.
   */
  void m_parse(XI::Container *services) final;

  /**
   * @brief Handles analyzing a module.
   * @param services                  Service container.
   */
  void m_analyze(XI::Container *services) final;

  /**
   * @brief Handles compiling a module.
   * @param services                  Service container.
   */
  void m_compile(XI::Container *services) final;
};

} // namespace Sabre::Module

#endif
