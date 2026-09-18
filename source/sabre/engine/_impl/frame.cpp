/// Sabre Includes
#include "sabre/engine/frame.hpp"
#include "sabre/runtime/isolate.hpp"

//  CONSTRUCTORS  //

Sabre::Engine::Frame::Frame(Isolate *isolate) : m_encoded(reinterpret_cast<uintptr_t>(isolate)) {
  isolate->m_frames.push_back(this);
}

Sabre::Engine::Frame::~Frame() {
  auto *isolate = m_isolate(); // decode the isolate
  if (isolate->thread()->task() == nullptr) return;
  $_ASSERT(isolate->m_frames.back() == this);
  isolate->m_frames.pop_back(); // remove frame
}
