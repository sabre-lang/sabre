/// Sabre Includes
#include "sabre/iterable/iterator.hpp"
#include "sabre/globals/roots.hpp"

//  PRIVATE METHODS  //

void Sabre::Iterable::Iterator::m_yield(const Iterator &self, Globals::Each &yield) {
  auto *attributes = self.m_wrapper();
  yield(attributes->state);
  yield(attributes->value);
}
