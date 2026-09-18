/// Builtin Includes
#include "sabre/builtins/_inline/apply.ipp"
#include "sabre/builtins/_inline/builtins.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Declarations
$_FWD(Sabre::Builtins::Constraint, static const $::Shared::Pointer<Type::Parameter> &T())

/// Forward Definitions
$_FWD(Sabre::Builtins, using TN = Type::New)
$_FWD(Sabre::Builtins, using Self = Type::Structure)

//  TYPEDEFS  //

#define SABRE_XX_FIELDS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Field, static Type::Entity N(const Self *))
#include "sabre/builtins/list/_defines/fields.def"

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Type::Entity N())
#include "sabre/builtins/list/_defines/statics.def"

//  PUBLIC METHODS  //

const $::Shared::Pointer<Sabre::Type::Parameter> &Sabre::Builtins::Constraint::T() {
  static auto s_T = TN::constraint("T", TN::any(), TN::any());
  return s_T; // define the necessary type-parameter now
}

$::Shared::Pointer<Sabre::Type::Generic> Sabre::Builtins::Wrapper<Sabre::Iterable::List>::generic() {
  return TN::generic(typeclass()->instantiate(), Constraint::T());
}

$::Shared::Pointer<Sabre::Type::Prototype> Sabre::Builtins::Wrapper<Sabre::Iterable::List>::typeclass() {
  return m_typeclass([](const $::Shared::Pointer<Type::Prototype> &prototype) {
    // bind the required constraints (List [T = Any])
    prototype->constraints() = {Constraint::T()};
  });
}

Sabre::Type::Entity Sabre::Builtins::Field::size(const Self *) { return TN::function(TN::number()); }
Sabre::Type::Entity Sabre::Builtins::Field::empty(const Self *) { return TN::function(TN::boolean()); }

Sabre::Type::Entity Sabre::Builtins::Field::get(const Self *self) {
  return TN::function(self->constraints(0), TN::number());
}

Sabre::Type::Entity Sabre::Builtins::Field::set(const Self *self) {
  return TN::function(self->constraints(0), TN::number(), self->constraints(0));
}

Sabre::Type::Entity Sabre::Builtins::Field::front(const Self *self) { return back(self); }
Sabre::Type::Entity Sabre::Builtins::Field::back(const Self *self) { return TN::function(self->constraints(0)); }

Sabre::Type::Entity Sabre::Builtins::Field::map(const Self *self) {
  // prepare the constraint
  auto V = TN::constraint("V", TN::any(), TN::any());

  // prepare the incoming values now
  auto index = TN::optional(TN::number());
  auto value = TN::optional(self->constraints(0));

  // prepare the mapping signature
  auto callback = TN::function(V, value, index);
  auto signature = TN::function(TN::list(V), callback);

  // and construct the resulting generic now
  return TN::generic(signature, V);
}

Sabre::Type::Entity Sabre::Builtins::Field::fold(const Self *self) {
  auto V = TN::constraint("V", TN::any(), TN::any());
  auto callback = TN::function(V, V, self->constraints(0));
  return TN::generic(TN::function(V, V, callback), V);
}

Sabre::Type::Entity Sabre::Builtins::Field::clear(const Self *self) {
  return TN::function(TN::list(self->constraints(0)));
}

Sabre::Type::Entity Sabre::Builtins::Field::drop(const Self *self) {
  return TN::function(TN::list(self->constraints(0)), TN::number());
}

Sabre::Type::Entity Sabre::Builtins::Field::erase(const Self *self) {
  return TN::function(TN::list(self->constraints(0)), TN::number(), TN::number());
}

Sabre::Type::Entity Sabre::Builtins::Field::slice(const Self *self) {
  auto start = TN::optional(TN::number()), end = TN::optional(TN::number());
  return TN::function(TN::list(self->constraints(0)), start, end);
}

Sabre::Type::Entity Sabre::Builtins::Field::filter(const Self *self) {
  auto value = TN::optional(self->constraints(0));
  auto callback = TN::optional(TN::function(TN::boolean(), value));
  return TN::function(TN::list(self->constraints(0)), callback);
}

Sabre::Type::Entity Sabre::Builtins::Field::reverse(const Self *self) {
  return {TN::function(TN::list(self->constraints(0)))};
}

Sabre::Type::Entity Sabre::Builtins::Field::push_front(const Self *self) { return push_back(self); }
Sabre::Type::Entity Sabre::Builtins::Field::push_back(const Self *self) {
  return TN::variadic(TN::number(), self->constraints(0));
}

Sabre::Type::Entity Sabre::Builtins::Field::pop_front(const Self *self) { return TN::function(self->constraints(0)); }
Sabre::Type::Entity Sabre::Builtins::Field::pop_back(const Self *self) { return TN::function(self->constraints(0)); }

Sabre::Type::Entity Sabre::Builtins::Field::first_index_of(const Self *self) { return last_index_of(self); }
Sabre::Type::Entity Sabre::Builtins::Field::last_index_of(const Self *self) {
  return TN::function(TN::number(), self->constraints(0));
}

Sabre::Type::Entity Sabre::Builtins::Static::from() {
  auto T = TN::constraint("T"); // bind "T"
  auto R = TN::list(TN::iterable(T, false));
  return TN::generic(TN::function(R, T), T);
}

Sabre::Type::Entity Sabre::Builtins::Static::empty() {
  auto T = TN::constraint("T", TN::any(), TN::any());
  return TN::generic(TN::function(TN::list(T)), T);
}

Sabre::Type::Entity Sabre::Builtins::Static::range() {
  auto index = TN::optional(TN::number());
  auto instance = TN::list(TN::number());
  auto args = std::vector<Type::Entity>({index, index, index});
  return TN::function(instance, args); // bind the function
}

Sabre::Type::Entity Sabre::Builtins::Static::filled() {
  auto V = TN::constraint("V"); // prepare the baseline constraint
  return TN::generic(TN::function(TN::list(V), TN::number(), V), V);
}

template <>
Sabre::Type::Erased
Sabre::Builtins::Apply<Sabre::Iterable::List>::unary(const Type::Structure *self, Operator::Kind kind) {
  switch (kind) {
  case Operator::Kind::ITER: return self->constraints(0);
  default: return TN::unset(); // resolve accordingly
  }
}

template <>
Sabre::Type::Erased
Sabre::Builtins::Apply<Sabre::Iterable::List>::binary(const Type::Structure *, Operator::Kind, const Type::Erased &) {
  return TN::unset();
}

//  PRIVATE METHODS  //

void Sabre::Builtins::Wrapper<Sabre::Iterable::List>::m_typedefs(Type::World *globals) {
  // prepare the baseline details
  auto prototype = typeclass();
  auto &fields = prototype->fields();
  auto &statics = prototype->statics();

  // bind the decision tree for operators
  prototype->operators() = Apply<Iterable::List>::decide;

// define the fields for symbols
#define SABRE_XX_FIELDS_DEFINE(N, ...) fields.emplace(#N, Field::N);
#include "sabre/builtins/list/_defines/fields.def"

// define the statics for symbols
#define SABRE_XX_STATICS_DEFINE(N, ...) statics.emplace(#N, Static::N());
#include "sabre/builtins/list/_defines/statics.def"

  // define the baseline types
  globals->values().declare(name(), prototype);
  globals->types().declare(name(), generic());
}
