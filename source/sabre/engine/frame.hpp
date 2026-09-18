#ifndef _SABRE_ENGINE_FRAME_HPP
#define _SABRE_ENGINE_FRAME_HPP

/// Sabre Includes
#include "sabre/engine/interrupt.hpp"
#include "sabre/forward/image.hpp"
#include "sabre/globals/roots.hpp"
#include "sabre/resource/trace.hpp"
#include "sabre/value/pointer.hpp"

namespace Sabre::Engine {

/// @brief Abstract Engine Frame.
struct $_ABSTRACT Frame : public $::RTTI::Dynamic {
  //  TYPEDEFS  //

  /// @brief Frame Abstraction Typing.
  template <class T> using Mixin = $::RTTI::Mixin<T, Frame>;

private:
  //  PROPERTIES  //

  /// @brief Runtime isolate.
  Pointer::Underlying m_encoded;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an engine frame.
   * @param isolate           Runtime isolate.
   */
  explicit Frame(Isolate *isolate);

  /// @brief Handles destructing frames.
  virtual ~Frame();

  //  PUBLIC METHODS  //

  /// @brief The underlying frame resource.
  virtual inline $::URI::View resource() const noexcept { return {}; }

  /// @brief Handles getting a frames backtrace.
  virtual inline Resource::Trace backtrace() const noexcept { return Resource::Trace(resource()); }

  /// @brief Gets the associated function arena.
  virtual inline const Image::Arena *arena() const noexcept { return nullptr; }

  /// @brief Yields all the available frame roots.
  virtual inline void roots(Globals::Each &) noexcept {};

  /// @brief Handles getting the current interrupted state.
  inline constexpr $::Enum::Flags<Interrupt> modes() const noexcept { return m_modes(); }

  /// @brief Sets a current interrupt for the frame.
  inline constexpr void interrupt(Interrupt mode = Interrupt::BAILOUT) noexcept { m_interrupt(mode); }

  /// @brief Allows casting to derived values.
  template <std::derived_from<Frame> T> inline constexpr T *as() noexcept { return $::RTTI::Cast<T>(this); }

  /// @brief Allows casting to derived values.
  template <std::derived_from<Frame> T> inline constexpr const T *as() const noexcept { return $::RTTI::Cast<T>(this); }

protected:
  //  PRIVATE METHODS  //

  /// @brief Gets the currently set interrupt flags.
  inline constexpr $::Enum::Flags<Interrupt> m_modes() const noexcept {
    auto flags = m_encoded & Pointer::Mask::IMM; // decode details
    return *reinterpret_cast<$::Enum::Flags<Interrupt> *>(&flags);
  }

  /// @brief Handles setting an interrupt flag.
  inline constexpr void m_interrupt(Interrupt mode) noexcept {
    m_encoded = (m_encoded & ~Pointer::Mask::IMM) | m_modes().set(mode).value();
  }

  /** Gets the internal isolate value. */
  inline constexpr Isolate *m_isolate() noexcept {
    return reinterpret_cast<Isolate *>(m_encoded & ~Pointer::Mask::IMM);
  }
};

} // namespace Sabre::Engine

#endif
