/// Sabre Includes
#include "sabre/machine/builder.hpp"

/// Machine Includes
#include "sabre/machine/_inline/glue.ipp"

/// Forward Declarations
$_FWD(Sabre::Machine, class Binary)
$_FWD(Sabre::Machine, class Compare)

/// Forward Definitions
$_FWD(Sabre::Machine, using Power = double (*)(double, double))

//  TYPEDEFS  //

/// @brief Prepare the binary handler.
class Sabre::Machine::Binary {
  //  TYPEDEFS  //

  /// @brief Available Bytecode Kinds.
  using Kind = Bytecode::Glyph;

  /// @brief Underlying qualified details.
  template <Kind::Encoded G> using Qualified = Bytecode::Qualified<G>;

protected:
  //  PROPERTIES  //

  Register::Host m_dx = {}; // Result register.
  Register::Host m_ax = {}; // Alpha register.
  Register::Host m_bx = {}; // Beta register.

  Double m_axv = {}; // Vector register alpha.
  Double m_bxv = {}; // Vector register beta.

  Register::Slot m_dv = {}; // Result bytecode register.
  Register::Slot m_av = {}; // Alpha bytecode register.

  /// @brief The bail label available.
  Label m_bail = {};

  //  CONSTRUCTORS  //

  /// @brief The underlying state to encompase.
  constexpr Binary(Builder *builder = nullptr) {
    // get the incoming registers now
    m_dx = builder->compiler->new_gp64("@dx");
    m_ax = builder->compiler->new_gp64("@ax");
    m_bx = builder->compiler->new_gp64("@bx");

    // prepare some necessary registers now
    m_axv = builder->compiler->new_vec128("@axv");
    m_bxv = builder->compiler->new_vec128("@bxv");

    // prepare the labels now
    m_bail = builder->compiler->new_label();
  }

  /// @brief The underlying state to encompase.
  constexpr Binary(Builder *builder, const Qualified<Kind::BINOP_TPL> *instruction) : Binary(builder) {
    m_dv = instruction->get<0>(); // bind result
    m_av = instruction->get<1>(); // and alpha
  }

  /// @brief The underlying state to encompase.
  constexpr Binary(Builder *builder, const Qualified<Kind::BINOP_TPLI> *instruction) : Binary(builder) {
    m_dv = instruction->get<0>(); // bind result
    m_av = instruction->get<1>(); // and alpha
  }

public:
  //  PUBLIC METHODS  //

  /// @brief Handles emitting binary operations.
  static inline void dispatch(Builder *builder, const Bytecode::Instruction *unqualified) noexcept {
    m_dispatch<Binary>(builder, unqualified);
  }

protected:
  //  PRIVATE METHODS  //

  /// @brief Handles dispatching the instance.
  template <std::derived_from<Binary> T>
  static inline constexpr void m_dispatch(Builder *builder, const Bytecode::Instruction *unqualified) {
    // prepare the baseline glyph to be used now
    auto glyph = unqualified->glyph();

    // and attempt dispatching as necessary now
    glyph.indexed() ? m_dispatch<T, Kind::BINOP_TPLI>(builder, glyph, unqualified)
                    : m_dispatch<T, Kind::BINOP_TPL>(builder, glyph, unqualified);
  }

  /// @brief Handles internal dispatching.
  template <std::derived_from<Binary> T, Kind::Encoded G>
  static inline constexpr void
  m_dispatch(Builder *builder, const Kind &kind, const Bytecode::Instruction *unqualified) {
    auto *instruction = unqualified->cast<G>(); // cast the instance as needd
    T(builder, instruction).m_generate(builder, kind, instruction->template get<2>());
  }

  /// @brief Handles generating results.
  template <class T>
  inline constexpr void m_generate(Builder *builder, const Kind &kind, const T &operand) const noexcept {
    // also bind a prolog that we may have
    m_prolog(builder);

    // preload the incoming binary instruction
    m_prepare(builder, operand);

    // and attempt applying the result now
    m_apply(builder, kind);
  }

  /// @brief Optional prolog instance.
  virtual inline constexpr void m_prolog(Builder *) const noexcept {}

  /// @brief Handles preloading the comparison.
  template <class T> inline constexpr void m_prepare(Builder *builder, const T &operand) const noexcept {
    // test whether or not the alpha register is valid
    builder->emitter->move(m_ax, m_av);
    builder->emitter->vtof(m_bail, m_axv, m_ax);

    // handle potential bytecode registers
    if constexpr (std::same_as<T, Register::Slot>) {
      builder->emitter->move(m_bx, operand);
      builder->emitter->vtof(m_bail, m_bxv, m_bx);
    }

    // handle potential immediate values
    if constexpr (std::same_as<T, Bytecode::Index>) {
      auto *arena = builder->info->arena();
      auto constant = arena->constants[operand];
      builder->emitter->load(m_bx, constant);
      builder->compiler->s_mov_u64(m_bxv, m_bx);
    }
  }

  /// @brief Handles incoming integral operations.
  inline constexpr void m_integral(Builder *builder, const Kind &kind) const noexcept {
    // prepare the start of the integral operation
    builder->compiler->s_cvt_round_f64_to_int(m_ax, m_axv);
    builder->compiler->s_cvt_round_f64_to_int(m_bx, m_bxv);

    // handle based on the incoming details now
    switch (kind.encoded()) {
    case Kind::BINOP_SHLI: $_FALLTHROUGH;
    case Kind::BINOP_SHL: builder->compiler->shl(m_ax, m_ax, m_bx); break;

    case Kind::BINOP_SHRI: $_FALLTHROUGH;
    case Kind::BINOP_SHR: builder->compiler->shr(m_ax, m_ax, m_bx); break;

    case Kind::BINOP_XORI: $_FALLTHROUGH;
    case Kind::BINOP_XOR: builder->compiler->xor_(m_ax, m_ax, m_bx); break;

    case Kind::BINOP_BORI: $_FALLTHROUGH;
    case Kind::BINOP_BOR: builder->compiler->or_(m_ax, m_ax, m_bx); break;

    case Kind::BINOP_BANDI: $_FALLTHROUGH;
    case Kind::BINOP_BAND: builder->compiler->and_(m_ax, m_ax, m_bx); break;

    default: $_ABORT("Unexpected operator '{0}'", kind.label()); break;
    }

    // prepare the post-condition of an integral operation
    builder->compiler->s_cvt_int_to_f64(m_axv, m_ax);
  }

  /// @brief Handles applying the incoming operation.
  virtual inline constexpr void m_apply(Builder *builder, const Kind &kind) const noexcept {
    // handle based on the incoming details now
    switch (kind.encoded()) {
    case Kind::BINOP_ADDI: $_FALLTHROUGH;
    case Kind::BINOP_ADD: builder->compiler->s_add_f64(m_axv, m_axv, m_bxv); break;

    case Kind::BINOP_SUBI: $_FALLTHROUGH;
    case Kind::BINOP_SUB: builder->compiler->s_sub_f64(m_axv, m_axv, m_bxv); break;

    case Kind::BINOP_MULI: $_FALLTHROUGH;
    case Kind::BINOP_MUL: builder->compiler->s_mul_f64(m_axv, m_axv, m_bxv); break;

    case Kind::BINOP_DIVI: $_FALLTHROUGH;
    case Kind::BINOP_DIV: builder->compiler->s_div_f64(m_axv, m_axv, m_bxv); break;

    case Kind::BINOP_MODI: $_FALLTHROUGH;
    case Kind::BINOP_MOD: builder->compiler->s_mod_f64(m_axv, m_axv, m_bxv); break;

    case Kind::BINOP_POWI: $_FALLTHROUGH;
    case Kind::BINOP_POW: builder->emitter->call<Power>(std::pow, m_axv, m_axv, m_bxv); break;

    // handle all incoming integral operations as well
    default: m_integral(builder, kind); break;
    }

    // cast the outgoing float back to a numeric
    builder->emitter->ftov(m_axv, m_dx);

    // and finally prepare our bail conditions to be used
    builder->compiler->bind(m_bail);
    builder->emitter->move(m_dv, m_dx);
  }
};

/// @brief Prepare the comparison handler.
class Sabre::Machine::Compare : public Binary {
  //  TYPEDEFS  //

  /// @brief Available Bytecode Kinds.
  using Kind = Bytecode::Glyph;

  /// @brief Underlying qualified details.
  template <Kind::Encoded G> using Qualified = Bytecode::Qualified<G>;

  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Binary::Binary;

public:
  //  PUBLIC METHODS  //

  /// @brief Handles emitting binary operations.
  static inline void dispatch(Builder *builder, const Bytecode::Instruction *unqualified) noexcept {
    m_dispatch<Compare>(builder, unqualified);
  }

protected:
  //  PRIVATE METHODS  //

  /// @brief Handles updating prolog details.
  inline constexpr void m_prolog(Builder *builder) const noexcept {
    builder->emitter->load(m_dx, Value::Boolean(false));
  }

  /// @brief Handles applying the condition.
  inline constexpr void m_apply(Builder *builder, const Kind &kind) const noexcept final {
    // handle based on the incoming condition given
    switch (kind.encoded()) {
    case Kind::TEST_LTI: $_FALLTHROUGH;
    case Kind::TEST_LT: builder->compiler->s_cmp_lt_f64(m_axv, m_axv, m_bxv); break;

    case Kind::TEST_GTI: $_FALLTHROUGH;
    case Kind::TEST_GT: builder->compiler->s_cmp_gt_f64(m_axv, m_axv, m_bxv); break;

    case Kind::TEST_LEI: $_FALLTHROUGH;
    case Kind::TEST_LE: builder->compiler->s_cmp_le_f64(m_axv, m_axv, m_bxv); break;

    case Kind::TEST_GEI: $_FALLTHROUGH;
    case Kind::TEST_GE: builder->compiler->s_cmp_ge_f64(m_axv, m_axv, m_bxv); break;

    // ignore when given invalid values at all
    default: $_ABORT("Unexpected operator '{0}'", kind.label()); break;
    }

    // preload the conditional value to move
    builder->emitter->load(m_bx, Value::Boolean(true));
    builder->compiler->s_mov(m_ax, m_axv);
    builder->compiler->cmov(m_dx, m_bx, asmjit::ujit::cmp_ne(m_ax, 0));

    // bind the resulting bail condition now
    builder->compiler->bind(m_bail), builder->emitter->move(m_dv, m_dx);
  }
};

//  PUBLIC METHODS  //

void Sabre::Machine::Emitter::unary(const Bytecode::Instruction *unqualified) {
  // destructure the incoming instruction
  auto *instruction = unqualified->cast<Bytecode::Glyph::UNOP_TPL>();

  // prepare the baseline bytecode registers
  auto dv = instruction->get<0>();
  auto tv = instruction->get<1>();

  // prepare all the incoming items to be used now
  auto dx = m_compiler->new_gp64();
  auto tx = m_compiler->new_gp64();

  // value float register
  auto txv = m_compiler->new_vec128();

  // prepare the bail details as well
  auto bail = m_compiler->new_label();

  // test whether or not the alpha register is valid
  move(tx, tv), vtof(bail, txv, tx);

  // apply the incoming operator now
  switch (instruction->glyph().encoded()) {
  // can simply negate values as needed
  case Bytecode::Glyph::UNOP_NEG: m_compiler->s_neg_f64(txv, txv); break;

  // inversion requires a little extra handling as an integer
  case Bytecode::Glyph::UNOP_INV: {
    m_compiler->s_cvt_round_f64_to_int(tx, txv);
    m_compiler->not_(tx, tx); // invert result
    m_compiler->s_cvt_int_to_f64(txv, tx);
  } break;

  // all unknown operators should follow through here
  default: $_ABORT("Unexpected operator '{0}'", instruction->glyph().label());
  }

  // cast the outgoing float back to a numeric
  ftov(txv, dx), m_compiler->bind(bail), move(dv, dx);
}

void Sabre::Machine::Emitter::binary(const Bytecode::Instruction *unqualified) {
  Binary::dispatch(m_builder, unqualified);
}

void Sabre::Machine::Emitter::logical(const Bytecode::Instruction *unqualified) {
  // get the underlying instruction instruction to be used
  auto *instruction = unqualified->cast<Bytecode::Glyph::BINOP_TPL>();

  // get the underlying details to be used
  auto dx = slot(instruction->get<0>());
  auto ax = slot(instruction->get<1>());
  auto bx = slot(instruction->get<2>());

  // and attempt running the logical invocation
  switch (instruction->glyph().encoded()) {
  case Bytecode::Glyph::BINOP_LOR: call(Glue::lor, dx, ax, bx); break;
  case Bytecode::Glyph::BINOP_LAND: call(Glue::land, dx, ax, bx); break;
  default: $_ABORT("Unexpected operator '{0}'", instruction->glyph().label()); break;
  }
}

void Sabre::Machine::Emitter::negate(const Bytecode::Instruction *unqualified) {
  // get the underlying instruction instruction to be used
  auto *instruction = unqualified->cast<Bytecode::Glyph::UNOP_TPL>();

  // get the underlying details to be used
  auto dv = instruction->get<0>();
  auto tv = instruction->get<1>();

  // get the associated registers as well
  auto dx = m_compiler->new_gp64();
  auto bail = m_compiler->new_label();

  // preload the incoming inverted value to be used
  load(dx, Value::Boolean(true));

  // attempt checking if we have a falsey value
  jmpf(bail, tv);

  // should safely emit the falsey value
  load(dx, Value::Boolean(false));

  // and bind the final label now
  m_compiler->bind(bail), move(dv, dx);
}

void Sabre::Machine::Emitter::coalesce(const Bytecode::Instruction *unqualified) {
  // get the underlying instruction instruction to be used
  auto *instruction = unqualified->cast<Bytecode::Glyph::BINOP_TPL>();

  // get the associated registers as well
  auto dx = m_compiler->new_gp64();
  auto bail = m_compiler->new_label();

  // load the incoming values to be used
  move(dx, instruction->get<1>());

  // check if the current value is void or not
  m_compiler->j(bail, asmjit::ujit::cmp_ne(dx, imm(Constants::Void)));

  // if we reached here, then we override the output value now
  move(dx, instruction->get<2>());

  // bind the label and resolve the destination value
  m_compiler->bind(bail), move(instruction->get<0>(), dx);
}

void Sabre::Machine::Emitter::equality(const Bytecode::Instruction *unqualified) {
  // get the underlying instruction instruction to be used
  auto *instruction = unqualified->cast<Bytecode::Glyph::BINOP_TPL>();

  // get the underlying details to be used
  auto dv = instruction->get<0>();
  auto av = instruction->get<1>();
  auto bv = instruction->get<2>();

  // get the associated registers as well
  auto dx = m_compiler->new_gp64();
  auto ax = m_compiler->new_gp64();
  auto bx = m_compiler->new_gp64();

  // prepare a bail out condition to be used
  auto bail = m_compiler->new_label();

  // prepare the baseline result to be used
  load(dx, Value::Boolean(true));

  // load the incoming "ax" and "bx" values
  move(ax, av), move(bx, bv);

  // attempt checking if we have equal values
  switch (instruction->glyph().encoded()) {
  case Bytecode::Glyph::TEST_EQ: m_compiler->j(bail, asmjit::ujit::cmp_eq(ax, bx)); break;
  case Bytecode::Glyph::TEST_NE: m_compiler->j(bail, asmjit::ujit::cmp_ne(ax, bx)); break;
  default: $_ABORT("Unexpected operator '{0}'", instruction->glyph().label()); break;
  }

  // slow path the false value if the result failed
  load(dx, Value::Boolean(false));

  // and bind the final label now
  m_compiler->bind(bail), move(dv, dx);
}

void Sabre::Machine::Emitter::compare(const Bytecode::Instruction *unqualified) {
  Compare::dispatch(m_builder, unqualified);
}
