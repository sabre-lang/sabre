#ifndef _SABRE_MODULE_DYNAMIC_HPP
#define _SABRE_MODULE_DYNAMIC_HPP

/// Sabre Includes
#include "sabre/module/abstract.hpp"

namespace Sabre::Module {

/// @brief Type Declaration Module.
struct Dynamic : public Mixin<Dynamic> {
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Mixin::Mixin;

  //  PUBLIC METHODS  //

  /// @brief Gets the associated library name.
  inline constexpr $::String::View name() const noexcept { return resource().body(); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles interpreting a module.
   * @param isolate                   Runtime isolate.
   * @param exports                   Exports metadata.
   */
  Async::Thenable *m_interpret(Runtime::Isolate *isolate, Metadata::Wrapper<Phase::EXPORTED> *exports) final;
};

} // namespace Sabre::Module

#endif
