/// Sabre Includes
#include "sabre/object/instance.hpp"
#include "sabre/globals/service.hpp"
#include "sabre/runtime/isolate.hpp"

/// Builtin Includes
#include "sabre/builtins/_inline/builtins.ipp"

//  CONSTRUCTORS  //

Sabre::Object::Wrapper<Sabre::Object::Instance>::Wrapper(Runtime::Isolate *isolate) {
  prototype = isolate->service<Globals::Service>()->prototype<Object::Instance>(isolate);
}

//  PRIVATE METHODS  //

void Sabre::Object::Instance::m_yield(const Instance &self, Globals::Each &yield) {
  // emplace the baseline details
  auto *attributes = self.m_wrapper();

  // yield the prototype instance now
  yield(attributes->prototype);

  // yield all the underlying member references as necessary now
  for (const auto &member : attributes->fields | std::views::values) yield(member->reference());
}
