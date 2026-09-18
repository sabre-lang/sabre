#ifndef _SABRE_ENGINE_METADATA_HPP
#define _SABRE_ENGINE_METADATA_HPP

/// Sabre Includes
#include "sabre/async/thenable.hpp"
#include "sabre/engine/exports.hpp"
#include "sabre/module/metadata.hpp"

namespace Sabre::Module {

/// @brief Constructs a set of export metadata.
template <> class Metadata::Wrapper<Phase::EXPORTED> : public Metadata, public Engine::Exports {
  //  PROPERTIES  //

  /// @brief The resolvable final exports.
  $::Unique::Pointer<Async::Thenable> m_thenable = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a set of engine metadata.
  explicit Wrapper() : m_thenable($::Unique::New<Async::Thenable>()) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying thenable instance.
  inline Async::Thenable *thenable() const noexcept { return m_thenable.get(); }

  /**
   * @brief Handles waiting for the final exports.
   * @param isolate               Runtime isolate.
   */
  inline Value::Any await(Runtime::Isolate *isolate) const noexcept { return thenable()->await(isolate); }
};

} // namespace Sabre::Module

#endif
