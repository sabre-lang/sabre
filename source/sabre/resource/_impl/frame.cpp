/// Sabre Includes
#include "sabre/resource/frame.hpp"
#include "sabre/runtime/isolate.hpp"

//  CONSTRUCTORS  //

Sabre::Resource::Frame::Frame(Runtime::Isolate *isolate, const Resource::Trace &trace) :
    Engine::Frame::Mixin<Frame>(isolate), m_trace(trace) {}

Sabre::Resource::Frame::Frame(Runtime::Isolate *isolate) : Engine::Frame::Mixin<Frame>(isolate) {
  if (auto *frame = isolate->frame()) m_trace = frame->backtrace();
}
