/// Sabre Modules
#include "sabre/bytecode/assembler.hpp"
#include "sabre/bytecode/routine.hpp"
#include "sabre/number/tagged.hpp"
#include "sabre/runtime/container.hpp"

//  CONSTRUCTORS  //

Sabre::Bytecode::Assembler::Assembler() : Assembler($::Global::get<Runtime::Container>()) {}
Sabre::Bytecode::Assembler::Assembler(XI::Container *) {}

//  PUBLIC METHODS  //

Sabre::Image::Slice Sabre::Bytecode::Assembler::process(Routine *routine, Image::Arena *arena) {
  // prepare the details to be used
  auto &binary = arena->binary;
  auto &records = binary.records();

  // prepare the starting offsets
  auto start = binary.size();
  Label::Encoded offset = start;

  // clear all the compilation details to be used now
  m_head = routine->head, m_placed = {}, m_unresolved = {};

  // prepare the output for our instructions now
  auto merged = std::vector<const Instruction *>();

  // attempt building the necessary offset labels and handle elisions
  for (const auto &block : routine->blocks) {
    for (const auto &label : block->labels()) m_placed.emplace(label, offset); // place labels now
    for (const auto &instruction : block->instructions()) merged.emplace_back(instruction.get());
    offset += Width * block->size(); // update the offset by total instructions sizing now
  }

  // attempt encoding all the merged instructions now
  for (const auto &instruction : merged) {
    auto pointer = binary.size(); // prepare
    binary.write(m_encode(instruction, pointer));
    auto *position = m_positions.get(instruction);
    if (position) records.append(pointer, *position);
  }

  // and return the resulting slice
  return m_patch(binary, arena), m_positions.clear(), binary.slice(start);
}

//  PRIVATE METHODS  //

void Sabre::Bytecode::Assembler::m_patch(Image::Binary &binary, Image::Arena *arena) const {
  // prepare the slot to be used
  auto slot = Index();

  // prepare the constants that are available
  auto &constants = arena->constants;

  // iterate over the available unresolved labels now
  for (const auto &[offset, entry] : m_unresolved) {
    // get the incoming details to be used
    auto [index, label] = entry;

    // validate the labels that we need to refer from
    $_ASSERT(m_placed.contains(index), "Unexpected patch-base '{0}'", index);
    $_ASSERT(m_placed.contains(label), "Unexpected patch-label '{0}'", label);

    // determine the relative offset now
    int64_t boff = m_placed.at(index);
    int64_t loff = m_placed.at(label);

    // prepare the jump index that we will use
    auto jump = Number::Tagged(boff - loff - static_cast<int64_t>(Width));
    $_ASSERT(jump.value() >= INT32_MIN && jump.value() <= INT32_MAX);

    // emplace the patch-constant now to be used
    auto iter = std::ranges::find(constants, Value::Any(jump));

    if (iter != constants.end()) slot = std::distance(constants.begin(), iter);
    else constants.push_back(jump), slot = constants.size() - 1; // resolve now

    // get the incoming patch-offset now
    auto peeked = binary.data()[offset];

    // attempt patching the incoming opcode now
    switch (peeked) {
#define SABRE_XX_GLYPH_JUMP(N, ...)                              \
  case Glyph::N: binary.patch(offset + 1, slot.encode()); break;
#include "sabre/bytecode/_defines/glyphs.def"
    default: $_ABORT("Unexpected opcode patch: {0}", peeked);
    }
  }
}

uint64_t Sabre::Bytecode::Assembler::m_encode(const Instruction *instruction, Label::Encoded offset) {
  // prepare an instruction label here
  auto label = m_head++;

  // and emplace the current label now
  m_placed.emplace(label, offset);

  // reserve some unresolved labels if necessary
  switch (instruction->glyph().encoded()) {
#define SABRE_XX_GLYPH_JUMP(N, ...)               \
  case Glyph::N: {                                \
    auto *jump = instruction->cast<Glyph::N>();   \
    Label::Patch entry = {jump->get<0>(), label}; \
    m_unresolved.emplace(offset, entry);          \
  } break;
#include "sabre/bytecode/_defines/glyphs.def"

  // regular values so we can ignore
  default: break;
  }

  // encode the incoming instruction now
  return instruction->encode();
}
