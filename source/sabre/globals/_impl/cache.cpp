/// Sabre Includes
#include "sabre/globals/cache.hpp"

/// Value Includes
#include "sabre/value/_inline/value.ipp"

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Globals::IC::get(const Value::Any &self, const Value::Symbol &symbol) noexcept {
  // get the incoming shape to be compared against
  auto shape = self.pointer().shape();

  // ensure that we validate each step of our incoming details
  if (shape >= Shape::Limits::BUILTINS) return Value::Failure();

  // get the baseline cache to be used and the spot
  const auto &cache = m_natives[shape];
  auto iter = cache.find(symbol), end = cache.end();

  // resolve as necessary now depending on status
  return iter == end ? Value::Failure() : iter->second;
}

bool Sabre::Globals::IC::set(const Value::Any &self, const Value::Symbol &symbol, const Value::Any &callee) noexcept {
  // get the incoming shape to be compared against
  auto shape = self.pointer().shape();

  // ensure that our shape is actually valid before continuing
  if (shape >= Shape::Limits::BUILTINS || !callee.is<Function::Native>()) return false;

  // we can safely promote our incoming value now
  return m_natives[shape].emplace(symbol, callee), true;
}
