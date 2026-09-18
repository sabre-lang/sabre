/// Engine Includes
#include "sabre/engine/_inline/macros.ipp"

//  X-MACROS  //

#define XX_SPECIALIZED_VALUES(X, ...) \
  X(Number::Tagged, __VA_ARGS__)      \
  X(String::Any, __VA_ARGS__)

#define XX_COMPARE_UNWRAP(T, OP, ...)                        \
  if (left.is<T>() && right.is<T>()) {                       \
    auto sink = instruction->get<0>();                       \
    auto result = left.as<T>().compare(right.as<T>()) OP 0;  \
    frame->store(sink, Value::Boolean(result));              \
    $_MUSTTAIL return tailcall(isolate, frame, unqualified); \
  } else

//  MACROS  //

#define MM_BINARY_EXECUTE(F, OP, MM, ...) MM_BINARY_##F(OP, OP, MM)
#define MM_BINARY_METHOD(F, L, R, ...) MM_BINARY_APPLY(L.as<Number::Tagged>().F(R.as<Number::Tagged>()))
#define MM_BINARY_OPERATOR(OP, L, R, ...) MM_BINARY_APPLY(L.as<Number::Tagged>() OP R.as<Number::Tagged>())
#define MM_BINARY_APPLY(...)                                 \
  $_PP_BLOCK({                                               \
    frame->store(instruction->get<0>(), __VA_ARGS__);        \
    $_MUSTTAIL return tailcall(isolate, frame, unqualified); \
  })

#define MM_BINARY_SLOW(OP, _, MM)                                                               \
  $_PP_BLOCK({                                                                                  \
    auto *instruction = unqualified->cast<Glyph::BINOP_TPL>();                                  \
    auto left = frame->load(instruction->get<1>()), right = frame->load(instruction->get<2>()); \
    if (left.is<Number::Tagged>() && right.is<Number::Tagged>()) MM(_, left, right);            \
    return isolate->panic(6000401, #OP, left.brand(), right.brand());                           \
  })

#define MM_BINARY_FAST(OP, _, MM)                                        \
  $_PP_BLOCK({                                                           \
    auto *instruction = unqualified->cast<Glyph::BINOP_TPLI>();          \
    auto left = frame->load(instruction->get<1>());                      \
    auto right = frame->constant<Number::Tagged>(instruction->get<2>()); \
    if (left.is<Number::Tagged>()) MM(_, left, right);                   \
    return isolate->panic(6000401, #OP, left.brand(), right.brand());    \
  })

#define MM_LOGICAL_HANDLER(OP, ...)                                                               \
  $_PP_BLOCK({                                                                                    \
    auto *instruction = unqualified->cast<Glyph::BINOP_TPL>();                                    \
    auto left = frame->load(instruction->get<1>()), right = frame->load(instruction->get<2>());   \
    frame->store(instruction->get<0>(), Value::Boolean(left.truthiness() OP right.truthiness())); \
    $_MUSTTAIL return tailcall(isolate, frame, unqualified);                                      \
  })

#define MM_EQUALITY_HANDLER(OP, ...)                                                            \
  $_PP_BLOCK({                                                                                  \
    auto *instruction = unqualified->cast<Glyph::BINOP_TPL>();                                  \
    auto left = frame->load(instruction->get<1>()), right = frame->load(instruction->get<2>()); \
    frame->store(instruction->get<0>(), Value::Boolean(left OP right));                         \
    $_MUSTTAIL return tailcall(isolate, frame, unqualified);                                    \
  })

#define MM_COMPARE_SLOW(OP, ...)                                                                \
  $_PP_BLOCK({                                                                                  \
    auto *instruction = unqualified->cast<Glyph::BINOP_TPL>();                                  \
    auto left = frame->load(instruction->get<1>()), right = frame->load(instruction->get<2>()); \
    XX_SPECIALIZED_VALUES(XX_COMPARE_UNWRAP, OP)                                                \
    frame->store(instruction->get<0>(), Value::False);                                          \
    $_MUSTTAIL return tailcall(isolate, frame, unqualified);                                    \
  })

#define MM_COMPARE_FAST(OP, ...)                                                                    \
  $_PP_BLOCK({                                                                                      \
    auto *instruction = unqualified->cast<Glyph::BINOP_TPLI>();                                     \
    auto left = frame->load(instruction->get<1>());                                                 \
    auto right = frame->constant<Number::Tagged>(instruction->get<2>());                            \
    Value::Boolean result = left.is<Number::Tagged>() ? left.as<Number::Tagged>() OP right : false; \
    frame->store(instruction->get<0>(), result);                                                    \
    $_MUSTTAIL return tailcall(isolate, frame, unqualified);                                        \
  })

//  PRIVATE METHODS  //

// template operators are only used for dynamic casting
SABRE_MM_ENGINE_EXECUTE(UNOP_TPL, , , ) { $::System::unreachable(); }
SABRE_MM_ENGINE_EXECUTE(BINOP_TPL, , , ) { $::System::unreachable(); }
SABRE_MM_ENGINE_EXECUTE(BINOP_TPLI, , , ) { $::System::unreachable(); }

SABRE_MM_ENGINE_EXECUTE(UNOP_INC, isolate, , ) { return isolate->todo("Unimplemented bytecode UNOP_INC"); }
SABRE_MM_ENGINE_EXECUTE(UNOP_DEC, isolate, , ) { return isolate->todo("Unimplemented bytecode UNOP_DEC"); }

SABRE_MM_ENGINE_EXECUTE(UNOP_NOT, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::UNOP_NOT>();
  auto truthiness = frame->load(instruction->get<1>()).truthiness();
  frame->store(instruction->get<0>(), Value::Boolean(!truthiness));
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(UNOP_NEG, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::UNOP_NEG>();
  auto operand = frame->load(instruction->get<1>()); // prepare the operand value now
  if (!operand.is<Number::Tagged>()) return isolate->panic(6000400, '-', operand.brand());
  frame->store(instruction->get<0>(), -operand.as<Number::Tagged>());
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(UNOP_INV, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::UNOP_INV>();
  auto operand = frame->load(instruction->get<1>()); // prepare the operand value now
  if (!operand.is<Number::Tagged>()) return isolate->panic(6000400, '~', operand.brand());
  frame->store(instruction->get<0>(), ~operand.as<Number::Tagged>());
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(BINOP_COAL, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::BINOP_COAL>(); // prepare the instruction
  auto left = frame->load(instruction->get<1>()), right = frame->load(instruction->get<2>());
  frame->store(instruction->get<0>(), left.is<Value::Void>() ? right : left);
  $_MUSTTAIL return tailcall(isolate, frame, unqualified);
}

SABRE_MM_ENGINE_EXECUTE(BINOP_ADD, isolate, frame, unqualified) {
  auto *instruction = unqualified->cast<Glyph::BINOP_ADD>(); // prepare the instruction
  auto left = frame->load(instruction->get<1>()), right = frame->load(instruction->get<2>());
  if (left.is<Number::Tagged>() && right.is<Number::Tagged>()) MM_BINARY_OPERATOR(+, left, right);
  else if (left.is<String::Any>() && right.is<String::Any>()) MM_BINARY_APPLY(concat(isolate, left, right));
  else return isolate->panic(6000402, left.brand(), right.brand()); // received a suitable failure
}

SABRE_MM_ENGINE_EXECUTE(BINOP_SUB, isolate, frame, unqualified) { MM_BINARY_EXECUTE(SLOW, -, MM_BINARY_OPERATOR); }
SABRE_MM_ENGINE_EXECUTE(BINOP_MUL, isolate, frame, unqualified) { MM_BINARY_EXECUTE(SLOW, *, MM_BINARY_OPERATOR); }
SABRE_MM_ENGINE_EXECUTE(BINOP_DIV, isolate, frame, unqualified) { MM_BINARY_EXECUTE(SLOW, /, MM_BINARY_OPERATOR); }
SABRE_MM_ENGINE_EXECUTE(BINOP_MOD, isolate, frame, unqualified) { MM_BINARY_EXECUTE(SLOW, %, MM_BINARY_OPERATOR); }
SABRE_MM_ENGINE_EXECUTE(BINOP_POW, isolate, frame, unqualified) { MM_BINARY_SLOW(**, power, MM_BINARY_METHOD); }

SABRE_MM_ENGINE_EXECUTE(BINOP_XOR, isolate, frame, unqualified) { MM_BINARY_EXECUTE(SLOW, ^, MM_BINARY_OPERATOR); }
SABRE_MM_ENGINE_EXECUTE(BINOP_BOR, isolate, frame, unqualified) { MM_BINARY_EXECUTE(SLOW, |, MM_BINARY_OPERATOR); }
SABRE_MM_ENGINE_EXECUTE(BINOP_BAND, isolate, frame, unqualified) { MM_BINARY_EXECUTE(SLOW, &, MM_BINARY_OPERATOR); }
SABRE_MM_ENGINE_EXECUTE(BINOP_SHL, isolate, frame, unqualified) { MM_BINARY_EXECUTE(SLOW, <<, MM_BINARY_OPERATOR); }
SABRE_MM_ENGINE_EXECUTE(BINOP_SHR, isolate, frame, unqualified) { MM_BINARY_EXECUTE(SLOW, >>, MM_BINARY_OPERATOR); }

SABRE_MM_ENGINE_EXECUTE(BINOP_ADDI, isolate, frame, unqualified) { MM_BINARY_EXECUTE(FAST, +, MM_BINARY_OPERATOR); }
SABRE_MM_ENGINE_EXECUTE(BINOP_SUBI, isolate, frame, unqualified) { MM_BINARY_EXECUTE(FAST, -, MM_BINARY_OPERATOR); }
SABRE_MM_ENGINE_EXECUTE(BINOP_MULI, isolate, frame, unqualified) { MM_BINARY_EXECUTE(FAST, *, MM_BINARY_OPERATOR); }
SABRE_MM_ENGINE_EXECUTE(BINOP_DIVI, isolate, frame, unqualified) { MM_BINARY_EXECUTE(FAST, /, MM_BINARY_OPERATOR); }
SABRE_MM_ENGINE_EXECUTE(BINOP_MODI, isolate, frame, unqualified) { MM_BINARY_EXECUTE(FAST, %, MM_BINARY_OPERATOR); }
SABRE_MM_ENGINE_EXECUTE(BINOP_POWI, isolate, frame, unqualified) { MM_BINARY_FAST(**, power, MM_BINARY_METHOD); }

SABRE_MM_ENGINE_EXECUTE(BINOP_XORI, isolate, frame, unqualified) { MM_BINARY_EXECUTE(FAST, ^, MM_BINARY_OPERATOR); }
SABRE_MM_ENGINE_EXECUTE(BINOP_BORI, isolate, frame, unqualified) { MM_BINARY_EXECUTE(FAST, |, MM_BINARY_OPERATOR); }
SABRE_MM_ENGINE_EXECUTE(BINOP_BANDI, isolate, frame, unqualified) { MM_BINARY_EXECUTE(FAST, &, MM_BINARY_OPERATOR); }
SABRE_MM_ENGINE_EXECUTE(BINOP_SHLI, isolate, frame, unqualified) { MM_BINARY_EXECUTE(FAST, <<, MM_BINARY_OPERATOR); }
SABRE_MM_ENGINE_EXECUTE(BINOP_SHRI, isolate, frame, unqualified) { MM_BINARY_EXECUTE(FAST, >>, MM_BINARY_OPERATOR); }

SABRE_MM_ENGINE_EXECUTE(BINOP_LOR, isolate, frame, unqualified) { MM_LOGICAL_HANDLER(&&); }
SABRE_MM_ENGINE_EXECUTE(BINOP_LAND, isolate, frame, unqualified) { MM_LOGICAL_HANDLER(||); }

SABRE_MM_ENGINE_EXECUTE(TEST_EQ, isolate, frame, unqualified) { MM_EQUALITY_HANDLER(==); }
SABRE_MM_ENGINE_EXECUTE(TEST_NE, isolate, frame, unqualified) { MM_EQUALITY_HANDLER(!=); }

SABRE_MM_ENGINE_EXECUTE(TEST_GT, isolate, frame, unqualified) { MM_COMPARE_SLOW(>); }
SABRE_MM_ENGINE_EXECUTE(TEST_LT, isolate, frame, unqualified) { MM_COMPARE_SLOW(<); }
SABRE_MM_ENGINE_EXECUTE(TEST_GE, isolate, frame, unqualified) { MM_COMPARE_SLOW(>=); }
SABRE_MM_ENGINE_EXECUTE(TEST_LE, isolate, frame, unqualified) { MM_COMPARE_SLOW(<=); }

SABRE_MM_ENGINE_EXECUTE(TEST_GTI, isolate, frame, unqualified) { MM_COMPARE_FAST(>); }
SABRE_MM_ENGINE_EXECUTE(TEST_LTI, isolate, frame, unqualified) { MM_COMPARE_FAST(<); }
SABRE_MM_ENGINE_EXECUTE(TEST_GEI, isolate, frame, unqualified) { MM_COMPARE_FAST(>=); }
SABRE_MM_ENGINE_EXECUTE(TEST_LEI, isolate, frame, unqualified) { MM_COMPARE_FAST(<=); }
