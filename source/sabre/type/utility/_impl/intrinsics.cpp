/// Builtins Includes
#include "sabre/builtins/_inline/builtins.ipp"

/// Sabre Includes
#include "sabre/type/_inline/type.ipp"

//  PUBLIC METHODS  //

Sabre::Type::Entity Sabre::Type::Any::lookup(const $::String::View &) const { return m_self(); }
Sabre::Type::Entity Sabre::Type::Unset::lookup(const $::String::View &) const { return m_self(); }

Sabre::Type::Erased Sabre::Type::Any::apply(Operator::Kind kind) const {
  switch (kind) {
  case Operator::Kind::NOT: return New::boolean();
  default: return m_self(); // default to "Any" type
  }
}

Sabre::Type::Erased Sabre::Type::Any::apply(Operator::Kind kind, const Erased &) const {
  switch (kind) {
  case Operator::Kind::EQ: $_FALLTHROUGH;
  case Operator::Kind::NE: $_FALLTHROUGH;
  case Operator::Kind::LOR: $_FALLTHROUGH;
  case Operator::Kind::LAND: return New::boolean();
  default: return m_self(); // default to "Any" type
  }
}

void Sabre::Type::None::m_print(std::ostream &os, const None &) { os << Builtins::Inspect<Value::Void>::name(); }
