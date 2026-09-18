/// Builtin Includes
#include "sabre/builtins/_inline/apply.ipp"
#include "sabre/builtins/_inline/builtins.ipp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

/// Forward Declarations
$_FWD(Sabre::Builtins::Constraint, static const $::Shared::Pointer<Type::Parameter> &T())
$_FWD(Sabre::Builtins::Static, static const $::Shared::Pointer<Type::Generic> &generator())

/// Forward Definitions
$_FWD(Sabre::Builtins, using TN = Type::New)
$_FWD(Sabre::Builtins, using Self = Type::Structure)

//  TYPEDEFS  //

#define SABRE_XX_FIELDS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Field, static Type::Entity N(const Self *))
#include "sabre/builtins/iterator/_defines/fields.def"

#define SABRE_XX_STATICS_DEFINE(N, ...) $_FWD(Sabre::Builtins::Static, static Type::Entity N())
#include "sabre/builtins/iterator/_defines/statics.def"

//  PUBLIC METHODS  //

const $::Shared::Pointer<Sabre::Type::Parameter> &Sabre::Builtins::Constraint::T() {
  static auto s_T = TN::constraint("T", TN::any(), TN::any());
  return s_T; // define the necessary type-parameter now
}

$::Shared::Pointer<Sabre::Type::Generic> Sabre::Builtins::Wrapper<Sabre::Iterable::Iterator>::generic() {
  return TN::generic(typeclass()->instantiate(), Constraint::T());
}

$::Shared::Pointer<Sabre::Type::Prototype> Sabre::Builtins::Wrapper<Sabre::Iterable::Iterator>::typeclass() {
  return m_typeclass([](const $::Shared::Pointer<Type::Prototype> &prototype) {
    // bind the required constraints (Iterator [T = Any])
    prototype->constraints() = {Constraint::T()};
  });
}

Sabre::Type::Entity Sabre::Builtins::Field::done(const Self *) { return TN::function(TN::boolean()); }
Sabre::Type::Entity Sabre::Builtins::Field::next(const Self *) { return TN::function(TN::boolean()); }
Sabre::Type::Entity Sabre::Builtins::Field::index(const Self *) { return TN::function(TN::number()); }
Sabre::Type::Entity Sabre::Builtins::Field::collect(const Self *self) {
  return TN::function(TN::list(self->constraints(0)));
}

Sabre::Type::Entity Sabre::Builtins::Static::empty() {
  auto T = TN::constraint("T", TN::any(), TN::any());
  return TN::generic(TN::function(TN::iterator(T)), T);
}

Sabre::Type::Entity Sabre::Builtins::Static::from() {
  // prepare the constraints to be used
  auto T = TN::constraint("T");
  auto R = TN::iterable(T, true);

  // construct the signature that resolves a suitable iterable
  return TN::generic(TN::function(R, T), T);
}

Sabre::Type::Entity Sabre::Builtins::Static::range() {
  auto index = TN::optional(TN::number());
  auto instance = TN::iterator(TN::number());
  auto args = std::vector<Type::Entity>({index, index, index});
  return TN::function(instance, args); // bind the function
}

const $::Shared::Pointer<Sabre::Type::Generic> &Sabre::Builtins::Static::generator() {
  // prepare the baseline typing to output
  static auto s_generator = $::Shared::Pointer<Type::Generic>();

  // stop early if already constructed here
  if (s_generator != nullptr) return s_generator;

  // prepare the context parameter to be used
  auto V = TN::constraint("V", TN::any(), TN::any());

  // prepare the yield handler to be used
  auto yield = TN::optional(TN::function(TN::none(), V));

  // prepare the arguments and signature to be used
  auto signature = TN::function(TN::boolean(), TN::number(), yield);

  // return the resulting context typing now
  return s_generator = TN::generic(signature, V);
}

Sabre::Type::Entity Sabre::Builtins::Static::dynamic() {
  // prepare the constraints to be used
  auto T = TN::constraint("T", TN::any(), TN::any());

  // prepare the signature to be used for the generator
  auto yield = generator()->instantiate({T});
  auto signature = TN::function(TN::iterator(T), yield);

  // allow binding the generics required for this instance
  return TN::generic(signature, T);
}

template <>
Sabre::Type::Erased
Sabre::Builtins::Apply<Sabre::Iterable::Iterator>::unary(const Type::Structure *self, Operator::Kind kind) {
  switch (kind) {
  case Operator::Kind::ITER: return self->constraints(0);
  default: return TN::unset(); // resolve accordingly
  }
}

template <>
Sabre::Type::Erased Sabre::Builtins::Apply<Sabre::Iterable::Iterator>::binary(
    const Type::Structure *, Operator::Kind, const Type::Erased &
) {
  return TN::unset();
}

//  PRIVATE METHODS  //

void Sabre::Builtins::Wrapper<Sabre::Iterable::Iterator>::m_typedefs(Type::World *globals) {
  // prepare the baseline details
  auto prototype = typeclass();
  auto &fields = prototype->fields();
  auto &statics = prototype->statics();

  // bind the decision tree for operators
  prototype->operators() = Apply<Iterable::Iterator>::decide;

// define the fields for symbols
#define SABRE_XX_FIELDS_DEFINE(N, ...) fields.emplace(#N, Field::N);
#include "sabre/builtins/iterator/_defines/fields.def"

// define the statics for symbols
#define SABRE_XX_STATICS_DEFINE(N, ...) statics.emplace(#N, Static::N());
#include "sabre/builtins/iterator/_defines/statics.def"

  // define the baseline types
  globals->values().declare(name(), prototype);
  globals->types().declare(name(), generic());
}
