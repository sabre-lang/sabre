/// Builtin Includes
#include "sabre/builtins/_inline/assert.ipp"

//  TYPEDEFS  //

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Value::Any N(Isolate *, const Args &))
#include "sabre/builtins/function/_defines/statics.def"

//  PUBLIC METHODS  //

const Sabre::Function::Info *Sabre::Builtins::Wrapper<Sabre::Function::Any>::glue() {
  // prepare the baseline items to be prepared
  static const Function::Info *s_info = nullptr;
  static $::URI::Buffer s_resource = $::URI::Evaluate("Function.bind()");
  static $::Shared::Pointer<Image::Arena> s_arena = $::Shared::New<Image::Arena>();

  // the expected bytecode to be emplaced
  static std::vector<Bytecode::Instruction> s_bytecode = {
      SABRE_MM_BIR(CLOSURE_PASS, Register::Accumulator),
      SABRE_MM_BIR(EXEC_RETURN), // and return the value
  };

  // if the information is available, then return now
  if (s_info != nullptr) return s_info;

  // prepare the shared information to be used
  auto shared = Bytecode::Shared();

  // ensure we define as variadic
  shared.locals = UINT32_MAX;
  shared.adicity = UINT64_MAX;

  // prepare the arena details to be used now
  s_arena->resource = s_resource;

  // construct a new set of function information to be used now
  auto info = $::Unique::New<Function::Info>(s_arena.get(), shared);

  // bind the bytecode to call the instance now
  for (const auto &instruction : s_bytecode) s_arena->binary.write(instruction.encode());

  // update the buffer before continuing now
  info->bytecode() = s_arena->binary.slice();

  // ensure the function is emplace as the "main" module now
  return s_info = s_arena->functions.emplace_back(std::move(info)).get();
}

Sabre::Value::Any Sabre::Builtins::Static::vlimit(Isolate *, const Args &) { return Number::Tagged(UINT64_MAX); }

Sabre::Value::Any Sabre::Builtins::Static::bind(Isolate *isolate, const Args &args) {
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 2);
  SABRE_MM_ASSERT_TYPEOF(isolate, Function::Any, args[0]);
  return isolate->bind(args.at<Function::Any>(0), args[1]);
}
Sabre::Value::Any Sabre::Builtins::Static::call(Isolate *isolate, const Args &args) {
  // ensure we have some valid values as necessary now
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, Function::Any, args[0]);

  // prepare the dynamic arguments now
  auto functor = args.at<Function::Any>(0);

  // attempt calling the instance now
  return isolate->invoke(functor, args.slice(1));
}

Sabre::Value::Any Sabre::Builtins::Static::apply(Isolate *isolate, const Args &args) {
  // ensure we have some valid values as necessary now
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, Function::Any, args[0]);

  // prepare the dynamic arguments and callable
  auto functor = args.at<Function::Any>(0);

  // this means we have a baseline callable value
  if (args.size() == 1) return isolate->invoke(functor);

  // ensure we now have an iterable list now
  SABRE_MM_ASSERT_TYPEOF(isolate, Iterable::List, args[1]);
  return isolate->invoke(functor, args.at<Iterable::List>(1).span());
}

//  PRIVATE METHODS  //

Sabre::Value::Any
Sabre::Builtins::Wrapper<Sabre::Function::Any>::m_globals(Isolate *isolate, const Object::Class &self) {
#define SABRE_XX_STATICS_DEFINE(N, ...)                                                \
  self.statics().emplace(#N, Member::Factory::native(isolate, Static::N, name(), #N));
#include "sabre/builtins/function/_defines/statics.def"

  // and return the resulting instance
  return self;
}
