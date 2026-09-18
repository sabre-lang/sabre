#ifndef _SABRE_RESOURCE_FRAME_HPP
#define _SABRE_RESOURCE_FRAME_HPP

/// Sabre Includes
#include "sabre/engine/frame.hpp"

namespace Sabre::Resource {

/// @brief Resource Only Engine Frame.
class Frame : public Engine::Frame::Mixin<Frame> {
  //  PROPERTIES  //

  /// @brief Associated resource trace.
  Trace m_trace;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a resource frame.
   * @param isolate           Runtime isolate.
   * @param trace             Resource trace.
   */
  explicit Frame(Runtime::Isolate *isolate);
  explicit Frame(Runtime::Isolate *isolate, const Trace &trace);

  //  PUBLIC METHODS  //

  inline constexpr Resource::Trace backtrace() const noexcept final { return m_trace; }
  inline constexpr $::URI::View resource() const noexcept final { return m_trace.resource(); }
};

} // namespace Sabre::Resource

#endif
