/// Sabre Includes
#include "sabre/object/enum.hpp"
#include "sabre/globals/roots.hpp"

//  PRIVATE METHODS  //

void Sabre::Object::Enum::m_yield(const Enum &self, Globals::Each &yield) {
  for (auto &variant : self.m_wrapper()->variants | std::views::values) {
    yield(variant.name), yield(variant.label), yield(variant.value->reference());
  }
}
