/// Sabre Includes
#include "sabre/engine/dispatch.hpp"
#include "sabre/engine/exports.hpp"
#include "sabre/engine/invoke.hpp"
#include "sabre/member/factory.hpp"
#include "sabre/runtime/isolate.hpp"

/// Value Includes
#include "sabre/value/_inline/value.ipp"

//  PUBLIC METHODS  //

Sabre::Value::Any Sabre::Engine::Dispatch::concat(Isolate *isolate, const Value::Any &left, const Value::Any &right) {
  return concat(isolate, left.as<String::Any>(), right.as<String::Any>());
}

Sabre::Value::Any Sabre::Engine::Dispatch::concat(Isolate *isolate, const String::Any &left, const String::Any &right) {
  return String::Any(isolate, "{0}{1}", left.view(), right.view());
}

Sabre::Value::Any Sabre::Engine::Dispatch::invoke(Isolate *isolate, const Value::Any &target, const Args &args) {
  return Invoke::dynamic(isolate, target, args);
}

Sabre::Value::Any Sabre::Engine::Dispatch::spawn(Isolate *isolate, const Value::Any &target, const Args &args) {
  if (!target.is<Function::Any>()) return isolate->panic(6000201, target.brand());
  return isolate->create<Async::Future>(target.as<Function::Any>(), args);
}

Sabre::Value::Any
Sabre::Engine::Dispatch::getter(Isolate *isolate, const Value::Any &target, const Value::Symbol &symbol) {
  auto descriptor = target.attribute(symbol); // get the descriptor to be used
  auto field = descriptor ? descriptor->getter(isolate, target) : Value::Missing();
  return m_feedback(isolate, {.target = target, .field = field, .symbol = symbol});
}

Sabre::Value::Any Sabre::Engine::Dispatch::setter(
    Isolate *isolate, const Value::Any &target, const Value::Any &value, const Value::Symbol &symbol
) {
  auto descriptor = target.attribute(symbol); // get the descriptor to be used
  auto field = descriptor ? descriptor->setter(isolate, target, value) : Value::Missing();
  return m_feedback(isolate, {.target = target, .field = field, .symbol = symbol});
}

Sabre::Value::Any Sabre::Engine::Dispatch::overrides(
    Isolate *isolate, const Value::Any &target, const Value::Any &callback, Operator::Kind kind
) {
  // ensure the callback is valid
  if (!callback.is<Function::Any>()) return isolate->panic(6000200, callback.brand());

  // resolve a suitable target for overloading
  auto name = Operator::Inspect::name(kind);
  auto symbol = Operator::Inspect::symbol(kind);

  // ensure we have a valid target for updating
  if (!target.is<Object::Instance>()) return isolate->panic(6000302, name, target.brand());

  // get the underlying instance now
  auto &fields = target.as<Object::Instance>().fields();

  // check if the field has already been assigned
  if (fields.contains(symbol)) return isolate->panic(6000303, name, target.brand());

  // can safely assign the necessary attribute now
  return fields.emplace(symbol, Member::Factory::reference(callback)), Value::Void();
}

Sabre::Value::Any Sabre::Engine::Dispatch::super(Isolate *isolate, const Object::Instance &instance, const Args &args) {
  // prepare the symbol to be used now
  static constexpr auto s_symbol = Operator::Attribute::CALL;

  // get the underlying parent constructor now
  auto parent = instance.prototype().parent();

  // fail if the parent is not a class
  if (!parent.is<Object::Class>()) return isolate->panic(6000203, parent.brand());

  // get the parent constructor details now
  auto prototype = parent.as<Object::Class>();
  const auto &statics = prototype.statics();

  // if the constructor is missing, then panic as well
  if (!statics.contains(s_symbol)) return isolate->panic(6000202, prototype.name());

  // can safely use the incoming self instance now
  auto constructor = statics.at(s_symbol)->getter(isolate, instance);
  return Invoke::dynamic(isolate, constructor, args); // and call now
}

Sabre::Value::Any Sabre::Engine::Dispatch::constructor(
    Isolate *isolate, const Object::Class &prototype, const Function::Info *info, const Value::Any &context
) {
  // prepare the symbol to be used now
  static constexpr auto s_symbol = Operator::Attribute::CALL;

  // prepare the statics as we will use these
  auto &statics = prototype.statics();

  // resolve the underlying constructor and bind it to the class
  auto closure = isolate->create<Function::Closure>(info, prototype, context);

  // prepare the constructor details
  if (statics.contains(s_symbol)) return isolate->panic(6000800, prototype.brand());
  return statics.emplace(s_symbol, Member::Factory::reference(closure)), Value::Void();
}

Sabre::Value::Any Sabre::Engine::Dispatch::member(
    Isolate *isolate,
    const Object::Instance &instance,
    const String::Intern *intern,
    const Value::Any &value,
    bool immutable
) {
  // get the baseline self value now to be used
  auto &fields = instance.fields();

  // attempt emplacing onto the available fields if possible to do so
  if (fields.contains(intern->symbol())) return isolate->panic(6000801, *intern);

  // prepare the reference to be bound now
  auto reference = Member::Factory::reference(value, immutable);
  return fields.emplace(intern->symbol(), std::move(reference)), Value::Void();
}

Sabre::Value::Any Sabre::Engine::Dispatch::object(Isolate *isolate, const Args &args) {
  return m_object(isolate, args.span());
}

Sabre::Value::Any Sabre::Engine::Dispatch::enumeration(Isolate *isolate, const Args &args) {
  return m_enumeration(isolate, args.span());
}

Sabre::Value::Any Sabre::Engine::Dispatch::iterator(Isolate *isolate, const Value::Any &iterable) {
  // check for any immediate iterators (as first-class values)
  if (iterable.is<Iterable::Iterator>()) return iterable;

  auto descriptor = iterable.attribute(Operator::Kind::ITER); // find the descriptor
  if (descriptor == nullptr) descriptor = iterable.attribute(Operator::Attribute::ITER);

  // attempt getting the underlying iterable value now
  auto iterator = descriptor ? descriptor->getter(isolate, iterable) : Value::Missing();
  return iterator.pointer().okay() ? iterator : isolate->panic(6000502, iterable.brand());
}

bool Sabre::Engine::Dispatch::matches(const Value::Any &value, const Value::Any &guard) {
  return m_matches<false>(value, guard) == Subtype::SUCCESS;
}

Sabre::Engine::Subtype Sabre::Engine::Dispatch::extends(const Value::Any &value, const Value::Any &guard) {
  return m_matches<true>(value, guard);
}

Sabre::Value::Any Sabre::Engine::Dispatch::ensure(Isolate *isolate, const Value::Any &value, const Value::Any &guard) {
  switch (extends(value, guard)) {
  case Subtype::SUCCESS: return value; // full match for our value here
  case Subtype::MISMATCH: return isolate->panic(3000353, guard.brand());
  default: return isolate->panic(3000352, value.brand(), guard.as<Object::Class>().name().view());
  }
}

Sabre::Value::Any
Sabre::Engine::Dispatch::barrel(Isolate *isolate, const Frame *frame, const Object::Instance &object) {
  // prepare the base output exports to be used
  auto *exports = isolate->exports(frame->resource().buffer());
  if (exports == nullptr) return Value::Failure(); // failed

  // get the underlying module instances to be barrel exported now
  auto &fields = exports->current().as<Object::Instance>().fields();

  // iterate over the available barrel fields now
  for (const auto &[key, value] : object.fields()) {
    if (fields.contains(key)) return isolate->panic(8000302); // failed
    fields.emplace(key, Member::Factory::reference(value->reference()));
  }

  // declare as a success now
  return Value::Void();
}

Sabre::Value::Any Sabre::Engine::Dispatch::expose(
    Isolate *isolate, const Frame *frame, const Value::Any &value, const String::Intern *intern
) {
  auto *exports = isolate->exports(frame->resource().buffer());
  return m_expose(isolate, exports, value, intern); // expose
}

//  PRIVATE METHODS  //

Sabre::Value::Any Sabre::Engine::Dispatch::m_interrupt(Isolate *isolate, Function::Frame *) {
  return isolate->panic(9000200);
}

bool Sabre::Engine::Dispatch::m_jump(Function::Frame *frame, const Bytecode::Index &index) {
  frame->offset() += frame->constant<Number::Tagged>(index).value();
  return frame->modes().test(Interrupt::BAILOUT); // test bailout
}

Sabre::Value::Any Sabre::Engine::Dispatch::m_object(Isolate *isolate, const std::span<Value::Any> &pairs) {
  // construct the baseline object instance
  auto object = isolate->create<Object::Instance>();

  // attempt assigning our values now
  for (size_t ii = 0; ii < pairs.size();) {
    auto field = pairs[ii++], value = pairs[ii++];
    auto member = Member::Factory::reference(value);
    object.fields()[field.as<Value::Symbol>()] = std::move(member);
  }

  // and return the object that was created
  return object;
}

Sabre::Value::Any Sabre::Engine::Dispatch::m_enumeration(Isolate *isolate, const std::span<Value::Any> &tuples) {
  // ensure we have some valid tuples
  $_ASSERT(tuples.size() % 3 == 0, "Expected tuples");

  // prepare the variants to be emplaced
  auto variants = std::vector<Object::Variant>();

  // prepare a resolution for incremental values
  static auto increment = [](const Object::Variant &variant) -> Number::Tagged {
    auto ordinal = variant.value->reference().as<Number::Tagged>();
    return Number::Tagged(ordinal.value() + 1); // increment ordinal
  };

  // attempt assigning our values now
  for (size_t ii = 0; ii < tuples.size();) {
    auto name = tuples[ii++].as<String::Any>();
    auto label = tuples[ii++].as<String::Any>();
    auto value = tuples[ii++]; // value needs resolving

    // check if the incoming value is void (eg: unassigned)
    if (value.is<Value::Void>()) value = variants.empty() ? Number::Zero : increment(variants.back());
    variants.emplace_back(Object::Variant{.name = name, .label = label, .value = Member::Factory::reference(value)});
  }

  // and finally construct and load the enumeration now
  return isolate->create<Object::Enum>(std::move(variants));
}

Sabre::Value::Any Sabre::Engine::Dispatch::m_invoke(Isolate *isolate, const Value::Symbol &symbol, const Args &args) {
  auto callee = getter(isolate, args.self(), symbol);
  if (!callee.pointer().okay()) return callee;
  return invoke(isolate, callee, args);
}

Sabre::Value::Any Sabre::Engine::Dispatch::m_spawn(Isolate *isolate, const Value::Symbol &symbol, const Args &args) {
  auto callee = getter(isolate, args.self(), symbol);
  if (!callee.pointer().okay()) return callee;
  return spawn(isolate, callee, args);
}

Sabre::Value::Any Sabre::Engine::Dispatch::m_expose(
    Isolate *isolate, Exports *exports, const Value::Any &value, const String::Intern *intern
) {
  if ($_UNLIKELY(exports == nullptr)) return Value::Failure(); // ignore empty
  return m_expose(isolate, exports->current().as<Object::Instance>(), value, intern);
}

Sabre::Value::Any Sabre::Engine::Dispatch::m_expose(
    Isolate *isolate, const Object::Instance &exports, const Value::Any &value, const String::Intern *intern
) {
  auto &fields = exports.fields(); // resolve the fields now
  auto exists = $_UNLIKELY(fields.contains(intern->symbol()));
  if (exists) return isolate->panic(8000301, intern->view());

  // construct and emplace the field to be used now
  auto reference = Member::Factory::reference(Value::Any(value));
  return fields.emplace(intern->symbol(), std::move(reference)), Value::Void();
}

Sabre::Value::Any Sabre::Engine::Dispatch::m_feedback(Isolate *isolate, const Feedback &feedback) {
  // validate the incoming target
  if (!feedback.target.pointer().okay()) return Value::Failure();

  // resolve a suitable feedback code to be used
  static $::Map::Base<Value::Feedback, Diagnostic::Code> s_codes = {
      {Value::Feedback::FIELD_MISSING, 6000300},
      {Value::Feedback::FIELD_IMMUTABLE, 6000301},
  };

  // fail immediately when feedback does not exist
  auto reference = feedback.field.pointer().feedback();
  if (!s_codes.contains(reference)) return feedback.field;

  // get the necessary details to be shown
  auto brand = feedback.target.brand();
  auto *intern = isolate->intern(feedback.symbol);

  // build the necessary panic to be returned
  if (intern) return isolate->panic(s_codes.at(reference), brand, intern->view());
  return isolate->panic(s_codes.at(reference), brand, feedback.symbol);
}

template <bool S>
Sabre::Engine::Subtype Sabre::Engine::Dispatch::m_matches(const Value::Any &value, const Value::Any &guard) {
  // check immediately against normal classes
  if (guard.is<Object::Class>()) return m_matches(value, guard.as<Object::Class>());

  /// TODO: otherwise we can also check against function guards (unsure yet)
  // if (guard.is<Function::Any>()) {}

  // depending on the strictness, handle non-guards as equality checks instead
  if constexpr (!S) return value == guard ? Subtype::SUCCESS : Subtype::FAILURE;

  // check for enumeration types now
  if (guard.is<Object::Enum>()) return value == guard ? Subtype::SUCCESS : Subtype::FAILURE;

  // otherwise we need to handle with some special cases now (special since non-class based)
  return guard.is<Value::Void>() && value.is<Value::Void>() ? Subtype::SUCCESS : Subtype::MISMATCH;
}

Sabre::Engine::Subtype Sabre::Engine::Dispatch::m_matches(const Value::Any &value, const Object::Class &guard) {
  // prepare the base shape to be compared against
  auto base = value.pointer().shape(), extends = guard.shape();

  // if we do not have a derived instance, then do a simple match against shapes
  if (!value.is<Object::Instance>()) return base == extends ? Subtype::SUCCESS : Subtype::FAILURE;

  // get the base instance to be checked against now
  auto instance = value.as<Object::Instance>();

  // otherwise we need to go through the prototype chain backwards
  for (auto prototype = instance.prototype();;) {
    // check for matches on the current prototype instance
    if (prototype.shape() == extends) return Subtype::SUCCESS;

    // get the next potential prototype
    auto parent = prototype.parent();

    // and break or continue if we have a super-type
    if (!parent.is<Object::Class>()) break;
    prototype = parent.as<Object::Class>();
  }

  // otherwise our matching failed
  return base == extends ? Subtype::SUCCESS : Subtype::FAILURE;
}
