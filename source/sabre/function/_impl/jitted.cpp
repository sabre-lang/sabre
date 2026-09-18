/// Sabre Includes
#include "sabre/function/jitted.hpp"
#include "sabre/globals/roots.hpp"

//  PRIVATE METHODS  //

void Sabre::Function::Jitted::m_yield(const Jitted &self, Globals::Each &yield) {
  auto *attributes = self.m_wrapper();
  yield(attributes->receiver);
  yield(attributes->context.environment());
}
