/// Sabre Includes
#include "sabre/iterable/list.hpp"
#include "sabre/globals/roots.hpp"

//  PRIVATE METHODS  //

void Sabre::Iterable::List::m_yield(const List &self, Globals::Each &yield) { yield(self.m_wrapper()->values); }
