/// Sabre Includes
#include "sabre/function/closure.hpp"
#include "sabre/globals/roots.hpp"

//  PRIVATE METHODS  //

void Sabre::Function::Closure::m_yield(const Closure &self, Globals::Each &yield) {
  auto *attributes = self.m_wrapper();
  yield(attributes->receiver);
  yield(attributes->context.environment());
}
