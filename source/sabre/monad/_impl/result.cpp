/// Sabre Includes
#include "sabre/monad/result.hpp"
#include "sabre/globals/roots.hpp"
#include "sabre/runtime/isolate.hpp"

//  PUBLIC METHODS  //

Sabre::Monad::Result Sabre::Monad::Success(Runtime::Isolate *isolate, const Value::Any &value) {
  return isolate->create<Result>(value, std::true_type());
}

Sabre::Monad::Result Sabre::Monad::Failure(Runtime::Isolate *isolate, const Value::Any &value) {
  return isolate->create<Result>(value, std::false_type());
}

//  PRIVATE METHODS  //

void Sabre::Monad::Result::m_yield(const Result &self, Globals::Each &yield) { yield(self.m_wrapper()->storage); }
